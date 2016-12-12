#include "Simulation.h"

#include <iostream>

#include "BoundaryBounceResolver.h"
#include "DragPhysicsHandler.h"
#include "IWorldPhysicsHandler.h"
#include "EulerMotionIntegrator.h"

#ifdef TRACING
#include "TracerConfig.h"
#include "tracing/TraceEvent.h"

using tracing::TraceEventType;
using tracing::TraceEvent;
#endif

Simulation::Simulation(SpatialContainer&& grid, double base_time_step):
        m_grid(std::move(grid)), m_base_time_step(base_time_step) {

    m_boundary_collision_resolver = make_default_boundary_resolver();
    m_world_physics = make_default_world_physics();
    m_integrator = make_default_integrator();
#ifdef TRACING
    m_tracer = build_tracer();
    setup_tracing();
#endif
}
 
Simulation& Simulation::set_boundary_collision_resolver(
        std::unique_ptr<IBoundaryCollisionResolver> resolver) {
    m_boundary_collision_resolver = std::move(resolver); 
    return *this;
}
 
Simulation::~Simulation() {
 
}
 
void Simulation::do_frame() {
    m_simulation_time.begin_frame(m_base_time_step);

    std::cout << "===== Frame Start: t=" << m_simulation_time.current_simulation_time()
        << " =====\n";

    SIM_TRACER_EVENT(m_tracer, TraceEventType::FrameBegin, this, m_simulation_time);

    for(int i = 0; i < m_grid.num_cells(); ++i) {
        auto& cell = m_grid.cell(i);
        for(auto& item : cell) {
            auto& particle = item;

            auto acceleration = compute_acceleration(particle.particle());

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
 
void Simulation::simulate_motion(Particle& particle, 
        double dt, const SpatialVector& acceleration) {
    auto position = particle.next_position();
    auto velocity = particle.next_velocity();
    m_integrator->advance_motion(*this, particle, dt, acceleration, position, velocity);
    particle.update_position(position);
    particle.update_velocity(velocity);
}

void Simulation::simulate_motion(Particle& particle, double dt, 
        SpatialVector acceleration, SpatialVector& position, 
        SpatialVector& velocity) {
    m_integrator->advance_motion(*this, particle, dt, acceleration, position, velocity);
}

std::unique_ptr<IBoundaryCollisionResolver> Simulation::make_default_boundary_resolver() {
    constexpr PositionType BOUNCE_COEFFICIENT = 0.95;
    return std::make_unique<BoundaryBounceResolver>(BOUNCE_COEFFICIENT);
}
 
std::unique_ptr<IWorldPhysicsHandler> Simulation::make_default_world_physics() {
    constexpr PositionType FRICTION_COEFFICIENT = 0.00; 
    return std::make_unique<DragPhysicsHandler>(FRICTION_COEFFICIENT);
}
 
std::unique_ptr<IMotionIntegrator> Simulation::make_default_integrator() {
    return std::make_unique<EulerMotionIntegrator>(); 
}
 
ForceType Simulation::compute_acceleration(Particle& particle) {
    return compute_acceleration(particle, particle.next_position(),
            particle.next_velocity());
}
 
ForceType Simulation::compute_acceleration(Particle& particle, 
    const SpatialVector& updated_position, const SpatialVector& updated_velocity) {
         
    auto force = compute_exact_force(particle, updated_position, updated_velocity);

    if(m_world_physics != nullptr) {
        auto world_force = 
            m_world_physics->compute_force(particle, *this, m_grid);
        force += world_force;
    }

    auto acceleration = compute_acceleration_from_force(particle, force);

    return acceleration; 
}
 
void Simulation::on_particle_out_of_boundry(Particle& particle,
        SpatialVector& acceleration) {
    PARTICLE_TRACER_EVENT(m_tracer, TraceEventType::WallCollideBegin, particle,
            this, m_simulation_time);

    m_boundary_collision_resolver->resolve_border_collision(*this, m_grid,
            particle, acceleration);

    PARTICLE_TRACER_EVENT(m_tracer, TraceEventType::WallCollideEnd, particle,
            this, m_simulation_time);
}

ForceType Simulation::compute_exact_force(const Particle& particle,
        const SpatialVector& position, const SpatialVector& velocity) {
    auto force = ForceType::zero();

    for(auto& item : m_grid) {
        auto& target = *item.second;
        if(&target.particle() == &particle) continue;
        force += particle.compute_force(target.particle(), position, velocity);
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

    simulate_motion(particle, dt, acceleration, new_position, new_velocity);

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
 
#ifdef TRACING
void Simulation::setup_tracing() {
    m_tracer.enable_particle_tracing(m_grid.get_particle_by_id(1));
}
#endif

 
