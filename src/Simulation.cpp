#include "Simulation.h"

#include <iostream>


#ifdef TRACING
#include "TracerConfig.h"
#include "tracing/TraceEvent.h"

using tracing::TraceEventType;
using tracing::TraceEvent;
#endif


Simulation::Simulation(SpatialContainer&& grid):
        m_grid(std::move(grid)) {

#ifdef TRACING
    m_tracer = build_tracer();
    setup_tracing();
#endif
}
 
void Simulation::do_frame() {
    m_simulation_time.begin_frame(0.1);

    std::cout << "===== Frame Start: t=" << m_simulation_time.current_simulation_time()
        << " =====\n";

    SIM_TRACER_EVENT(m_tracer, TraceEventType::FrameBegin, this, m_simulation_time);

    for(int i = 0; i < m_grid.num_cells(); ++i) {
        auto& cell = m_grid.cell(i);
        for(auto& item : cell) {
            auto& particle = item;

            auto acceleration = compute_acceleration_from_force(particle.particle(),
                    compute_exact_force(particle.particle()));

            advance_physics(particle.particle(), m_simulation_time.time_delta(), 
                    acceleration);
        }
    }

    m_grid.next_frame();
    for(auto& particle : m_grid) {
        m_grid.update_particle(*particle.second);
    }

    SIM_TRACER_EVENT(m_tracer, TraceEventType::FrameEnd, this, m_simulation_time);
}
 
void Simulation::on_particle_out_of_boundry(Particle& particle,
        SpatialVector& acceleration) {
    PARTICLE_TRACER_EVENT(m_tracer, TraceEventType::WallCollideBegin, particle,
            this, m_simulation_time);

    resolve_border_collision(particle,acceleration);

    PARTICLE_TRACER_EVENT(m_tracer, TraceEventType::WallCollideEnd, particle,
            this, m_simulation_time);
}

void Simulation::resolve_border_collision(Particle& particle, 
        SpatialVector& acceleration) {
    double remaining_time = m_simulation_time.time_delta();

    resolve_border_collision_recursive(particle, remaining_time, acceleration);

    if(remaining_time > 0) {
        PARTICLE_TRACER_EVENT(m_tracer, TraceEventType::SimulateRemainder,
            particle, this, m_simulation_time);
        PARTICLE_MOTION_TRACER_EVENT(m_tracer, TraceEventType::MotionParamsUpdated, particle,
            this, m_simulation_time, remaining_time, acceleration, ( 
                [this, &particle, remaining_time, &acceleration]() {
                    euler(particle, remaining_time, acceleration);
                }
            ));
    }
}

void Simulation::resolve_border_collision_recursive(Particle& particle, 
        double& remaining_time, SpatialVector& acceleration) {

    auto velocity = particle.next_velocity();
    auto pos = particle.next_position();

    double x_time;
    double y_time;

    if(velocity.x < 0) {
        x_time = -pos.x / velocity.x;
    } else {
        x_time = (m_grid.width() - pos.x) / velocity.x;
    }

    if(velocity.y < 0) {
        y_time = -pos.y / velocity.y;
    } else {
        y_time = (m_grid.height() - pos.y) / velocity.y;
    }

    if(x_time >= 0 && x_time < y_time) {
        if(x_time < remaining_time) {
            PARTICLE_TRACER_EVENT(m_tracer, TraceEventType::WallBounce,
                    particle, this, m_simulation_time);

            PARTICLE_MOTION_TRACER_EVENT(m_tracer, TraceEventType::MotionParamsUpdated, 
                    particle, this, m_simulation_time, x_time, acceleration, ( 
                [this, &particle, x_time, &acceleration]() {
                    euler(particle, x_time, acceleration);
                    particle.next_velocity().x = -particle.next_velocity().x;
                }
            ));

            remaining_time -= x_time;
            resolve_border_collision_recursive(particle, remaining_time,
                acceleration);
        }
    } else if(y_time >= 0) {
        if(y_time < remaining_time) {
            PARTICLE_TRACER_EVENT(m_tracer, TraceEventType::WallBounce,
                    particle, this, m_simulation_time);

            PARTICLE_MOTION_TRACER_EVENT(m_tracer, TraceEventType::MotionParamsUpdated, 
                    particle, this, m_simulation_time, y_time, acceleration, ( 
                [this, &particle, y_time, &acceleration]() {
                    euler(particle, y_time, acceleration); 
                    particle.next_velocity().y = -particle.next_velocity().y;
                }
            ));

            remaining_time -= y_time;
            resolve_border_collision_recursive(particle, remaining_time,
                acceleration);
        }
    }
}
 
ForceType Simulation::compute_exact_force(const Particle& particle) {
    auto force = ForceType::zero();

    for(auto& item : m_grid) {
        auto& target = *item.second;
        if(&target.particle() == &particle) continue;
        force += particle.compute_force(target.particle());
    } 

    return force;
}
 
ForceType Simulation::compute_acceleration_from_force(const Particle& particle, 
        const ForceType& force) const {
    return force / particle.mass(); 
}
 
void Simulation::advance_physics(Particle& particle, double dt, SpatialVector acceleration) {
    PARTICLE_TRACER_EVENT(m_tracer, TraceEventType::ParticleFrameBegin, particle, 
            this, m_simulation_time)
        
    auto new_position = particle.next_position();
    auto new_velocity = particle.next_velocity();
    euler(particle, dt, acceleration, new_position, new_velocity);

    if(!m_grid.is_point_within(new_position)) {
        //If we detect a boundary collision, we discard the simulation and
        //run a more detailed simulation that breaks the movement up into segments
        //between each collision.
        on_particle_out_of_boundry(particle, acceleration);
    } else {
        PARTICLE_MOTION_TRACER_EVENT(m_tracer, TraceEventType::MotionParamsUpdated, 
            particle, this, m_simulation_time, dt, acceleration, ( 
                [this, &particle, &new_position, &new_velocity]() {
                    particle.update_velocity(new_velocity);
                    particle.update_position(new_position);
                }
            ));
    }

    PARTICLE_TRACER_EVENT(m_tracer, TraceEventType::ParticleFrameEnd, particle, 
            this, m_simulation_time);
}
 
void Simulation::euler(Particle& particle, double dt, SpatialVector acceleration) {
    auto vel = particle.next_velocity() + acceleration*dt;
    auto pos = particle.next_position() + particle.next_velocity()*dt; 

    particle.update_velocity(vel);
    particle.update_position(pos);
}

void Simulation::euler(const Particle& particle, double dt, SpatialVector acceleration, 
        SpatialVector& position, SpatialVector& velocity) { 
    velocity = velocity + acceleration*dt;
    position = position + particle.next_velocity()*dt; 
}
 
#ifdef TRACING
void Simulation::setup_tracing() {
    m_tracer.enable_particle_tracing(m_grid.get_particle_by_id(1));
}
#endif
 
 
