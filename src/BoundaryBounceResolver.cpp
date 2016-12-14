#include "BoundaryBounceResolver.h"

#include <cassert>
#include <iostream>

#include "Grid.h"
#include "Simulation.h"
#include "Vector2.h"
#include "tracing/Tracer.h"
#include "tracing/TraceEvent.h"

using namespace tracing;

BoundaryBounceResolver::BoundaryBounceResolver(PositionType bounce_coefficient):
    m_bounce_coeff(bounce_coefficient) {}
 
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

    /*std::cout << "\tFinal division: \n\tt: uncertainty: [" 
        << current_min_time << ", " << current_max_time
        << "]  final: " << end_dt
        << " -- " << "pos=" << next_position << std::endl;*/

    double end_dt = results.end_time;

    simulation.simulate_motion(particle, end_dt, acc, 
        next_position, next_velocity);

    remaining_time -= end_dt;

    std::cout << "\t\tPre-Interp: " << next_position << ", " << next_velocity
        << "\n";

    auto result = 
        interpolate_to_boundary(
            simulation, particle, grid, remaining_time, next_position,
            next_velocity);
    std::tie(next_position, next_velocity) = result;

    std::cout << "\t\tPost-Interp: " << next_position << ", " << next_velocity
        << "\n";

    //std::cout << "\tCorrected Endpoint: pos=" << next_position << " -- vel="
    //    << next_velocity << "\n";

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
        std::cout << "\tRemaining Time: " << remaining_time << "\n";
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

    /*auto dx_left = std::abs(next_position.x);
    auto dx_right = std::abs(next_position.x - grid.width());
    auto dy_top = std::abs(next_position.y);
    auto dy_bottom = std::abs(next_position.y - grid.height());

    auto dx_min = std::min(dx_left, dx_right);
    auto dy_min = std::min(dy_top, dy_bottom);*/

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

    /*
    double time = 0.0;
    auto vel = next_velocity;
    auto pos = next_position;
    if(dx_min < dy_min) {
        if(dx_left < dx_right) {
            time = -pos.x / vel.x; 
            std::cout << "Interpolation: dt=" << time << "; pos="
                << next_position;
            next_position.y += time*vel.y;
            next_position.x = 0.0;
            std::cout << "->" << next_position << "\n";
        } else {
            time = (grid.width() - pos.x) / vel.x;
            std::cout << "Interpolation: dt=" << time << "; pos="
                << next_position;
            next_position.y += time*vel.y;
            next_position.x = grid.width() - 1e-6;
            std::cout << "->" << next_position << "\n";
        }

        next_velocity.x = -next_velocity.x;
        remaining_time -= time;
    } else {
        if(dy_top < dy_bottom) {
            time = -pos.y / vel.y;
            std::cout << "Interpolation: dt=" << time << "; pos="
                << next_position;
            next_position.x += time*vel.x;
            next_position.y = 0.0;    
            std::cout << "->" << next_position << "\n";
        } else {
            time = (grid.height() - pos.y) / vel.y;
            std::cout << "Interpolation: dt=" << time << "; pos="
                << next_position;
            next_position.x += time*vel.x;
            next_position.y = grid.height() - 1e-6;    
            std::cout << "->" << next_position << "\n";
        }
        next_velocity.y = -next_velocity.y;
        remaining_time -= time;
    }*/

    assert(grid.is_point_within(next_position));
    std::cout << "\tRemaining time: " << remaining_time << std::endl;

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

    static constexpr int ITERATION_DEPTH = 3;

    //std::cout << "WALL COLLISION START: \n";
    //std::cout << "\tInitial: pos=" << particle.next_position() <<
    //    " -- vel=" << particle.next_velocity() << "\n";

    for(int i = 0; i < ITERATION_DEPTH; ++i) {
        next_position = particle.next_position();
        next_velocity = particle.next_velocity();

        next_dt = (current_max_time + current_min_time) * 0.5;
        //std::cout << "\tdt = " << next_dt << "\n";
        simulation.simulate_motion(particle, next_dt, acceleration, 
                next_position, next_velocity);

        //std::cout << "\tpos=" << particle.next_position() << "->" << next_position
        //    << "\n";
        if(!grid.is_point_within(next_position)) {
            //std::cout << "\tOutside\n";
            current_max_time = next_dt; 
            end_dt = (current_max_time + current_min_time) * 0.5;
        } else {
            //std::cout << "\tInside\n";
            current_min_time = next_dt; 
            end_dt = (current_max_time + current_min_time) * 0.5;
        }
    }

    SubdivideResults results;
    results.end_time = end_dt;
    results.min_time = current_min_time;
    results.max_time = current_max_time;
    results.final_pos = next_position;
    results.final_vel = next_velocity;

    return results;
}
 
