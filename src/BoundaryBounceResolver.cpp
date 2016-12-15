#include "BoundaryBounceResolver.h"

#include <cassert>

#include "Grid.h"
#include "Simulation.h"
#include "Vector2.h"
#include "tracing/Tracer.h"
#include "tracing/TraceEvent.h"

using namespace tracing;

BoundaryBounceResolver::BoundaryBounceResolver(PositionType bounce_coefficient,
        int division_levels):
    m_bounce_coeff(bounce_coefficient), m_division_levels(division_levels) {}
 
void BoundaryBounceResolver::resolve_border_collision(Simulation& simulation, 
        Grid& grid, Particle& particle, SpatialVector& acceleration) const {
    double dt = simulation.simulation_time().time_delta();
    double remaining_time = dt;     

    resolve_border_collision_recursive(simulation, grid, particle, acceleration,
            remaining_time);

    assert(remaining_time <= 0);

    PARTICLE_TRACER_EVENT(simulation.tracer(), TraceEventType::CollisionEnergyLoss,
        particle, &simulation, simulation.simulation_time());

    PARTICLE_MOTION_TRACER_EVENT(simulation.tracer(), 
            TraceEventType::MotionParamsUpdated, particle,
        &simulation, simulation.simulation_time(), 0.0, acceleration, ( 
            [this, &particle]() {
                particle.update_velocity(particle.next_velocity() * m_bounce_coeff);
            }
        ));
}
 
void BoundaryBounceResolver::resolve_border_collision_recursive(Simulation& simulation, 
        Grid& grid, Particle& particle, SpatialVector& acceleration, 
        double& remaining_time, int recursion_count) const {
    assert(recursion_count < 10);

    double start_rem_time = remaining_time;

    auto next_position = particle.next_position();
    auto next_velocity = particle.next_velocity();
    auto acc = acceleration;

    auto results = boundary_collision_subdivide(simulation, 
        particle, grid, acc, remaining_time, next_position,
        next_velocity);    

    double end_dt = results.end_time;

    simulation.simulate_motion(particle, end_dt, acc, 
        next_position, next_velocity);

    remaining_time -= end_dt;


    auto result = 
        interpolate_to_boundary(
            simulation, particle, grid, remaining_time, next_position,
            next_velocity);
    std::tie(next_position, next_velocity) = result;

    PARTICLE_MOTION_TRACER_EVENT(simulation.tracer(), 
            TraceEventType::MotionParamsUpdated, particle,
        &simulation, simulation.simulation_time(), start_rem_time-remaining_time, 
        acceleration, ( 
            [&particle, &next_position, &next_velocity]() {
                particle.update_position(next_position);
                particle.update_velocity(next_velocity);
            }
        ));

    if(remaining_time >= 0) {
        if(!try_simulate_remainder(simulation, particle, grid, acceleration, 
                remaining_time, next_position, next_velocity)) {
            resolve_border_collision_recursive(simulation, grid, particle,
                acceleration, remaining_time, recursion_count+1);
        }
    }
}
 
bool BoundaryBounceResolver::try_simulate_remainder(Simulation& simulation, 
        Particle& particle, Grid& grid, SpatialVector& acceleration, 
        double& remaining_time, const SpatialVector& start_pos, 
        const SpatialVector& start_vel) const {
 
    auto start_time = remaining_time;
    auto test_pos = start_pos;
    auto test_vel = start_vel;

    simulation.simulate_motion(particle, remaining_time, acceleration, 
            test_pos, test_vel);

    if(!grid.is_point_within(test_pos)) {
        remaining_time = start_time;
        return false;
    } else {
        PARTICLE_TRACER_EVENT(simulation.tracer(), TraceEventType::SimulateRemainder,
            particle, &simulation, simulation.simulation_time());

        PARTICLE_MOTION_TRACER_EVENT(simulation.tracer(), 
                TraceEventType::MotionParamsUpdated, particle,
            &simulation, simulation.simulation_time(), remaining_time, acceleration, ( 
                [this, &particle, &test_pos, &test_vel]() {
                    particle.update_position(test_pos);
                    particle.update_velocity(test_vel);
                }
            ));
        remaining_time = 0.0; 

        return true;
    }
}

std::tuple<SpatialVector, SpatialVector> 
    BoundaryBounceResolver::interpolate_to_boundary(Simulation& simulation, 
        Particle& particle, Grid& grid, double& remaining_time, 
        const SpatialVector& start_pos, const SpatialVector& start_vel) const {
     
    auto next_position = start_pos;
    auto next_velocity = start_vel;

    double x_time;
    double y_time;

    if(next_velocity.x < 0) {
        x_time = -next_position.x / next_velocity.x;
    } else {
        x_time = (grid.width() - next_position.x) / next_velocity.x;
    }

    if(next_velocity.y < 0) {
        y_time = -next_position.y / next_velocity.y;
    } else {
        y_time = (grid.height() - next_position.y) / next_velocity.y;
    }

    if(std::abs(x_time) < std::abs(y_time)) {
        if(x_time > remaining_time) {
            x_time = remaining_time;
        }
        remaining_time -= x_time;
        next_position += PositionType(x_time)*next_velocity;

        if(next_velocity.x < 0) {
            next_position.x = 0.0;
        } else {
            next_position.x = grid.width() - 1e-6;
        }

        next_velocity.x = -next_velocity.x;

    } else {
        if(y_time > remaining_time) {
            y_time = remaining_time;
        }
        remaining_time -= y_time;
        next_position += PositionType(y_time)*next_velocity;

        if(next_velocity.y < 0) {
            next_position.y = 0.0;
        } else {
            next_position.y = grid.height() - 1e-6;
        }

        next_velocity.y = -next_velocity.y;
    }
    next_position = grid.clip_outer_boundary(next_position);

    assert(grid.is_point_within(next_position));

    return std::make_tuple(next_position, next_velocity);
}

SubdivideResults BoundaryBounceResolver::boundary_collision_subdivide(
        Simulation& simulation, Particle& particle, Grid& grid, 
        SpatialVector& acceleration, double& remaining_time, 
        const SpatialVector& position, const SpatialVector& velocity) const {
    double end_dt = 0.0;
    double next_dt = 0.0;
    double current_min_time = 0.0;
    double current_max_time = remaining_time;

    auto next_position = particle.next_position();
    auto next_velocity = particle.next_velocity();

    for(int i = 0; i < m_division_levels; ++i) {
        next_position = particle.next_position();
        next_velocity = particle.next_velocity();

        next_dt = (current_max_time + current_min_time) * 0.5;
        simulation.simulate_motion(particle, next_dt, acceleration, 
                next_position, next_velocity);

        if(!grid.is_point_within(next_position)) {
            current_max_time = next_dt; 
        } else {
            current_min_time = next_dt; 
        }
        end_dt = (current_max_time + current_min_time) * 0.5;
    }

    SubdivideResults results;
    results.end_time = end_dt;
    results.min_time = current_min_time;
    results.max_time = current_max_time;
    results.final_pos = next_position;
    results.final_vel = next_velocity;

    return results;
}
 
