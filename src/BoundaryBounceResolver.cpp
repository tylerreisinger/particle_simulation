#include "BoundaryBounceResolver.h"

#include "Grid.h"
#include "Simulation.h"
#include "Vector2.h"
#include "tracing/Tracer.h"
#include "tracing/TraceEvent.h"

using namespace tracing;

void BoundaryBounceResolver::resolve_border_collision(Simulation& simulation, Grid& grid,
        Particle& particle, SpatialVector& acceleration) const {
    double dt = simulation.simulation_time().time_delta();
    double remaining_time = dt;

    resolve_border_collision_recursive(simulation, grid, particle, 
            acceleration, remaining_time);

    if(remaining_time > 0) {
        PARTICLE_TRACER_EVENT(simulation.tracer(), TraceEventType::SimulateRemainder,
            particle, &simulation, simulation.simulation_time());

        PARTICLE_MOTION_TRACER_EVENT(simulation.tracer(), 
                TraceEventType::MotionParamsUpdated, particle,
            &simulation, simulation.simulation_time(), remaining_time, acceleration, ( 
                [this, &simulation, &particle, remaining_time, &acceleration]() {
                    simulation.simulate_motion(particle, remaining_time, acceleration);
                }
            ));
    }
}
 
void BoundaryBounceResolver::resolve_border_collision_recursive(Simulation& simulation, 
        Grid& grid, Particle& particle, SpatialVector& acceleration, 
        double& remaining_time) const {

    auto velocity = particle.next_velocity();
    auto pos = particle.next_position();

    double x_time;
    double y_time;

    if(velocity.x < 0) {
        x_time = -pos.x / velocity.x;
    } else {
        x_time = (grid.width() - pos.x) / velocity.x;
    }

    if(velocity.y < 0) {
        y_time = -pos.y / velocity.y;
    } else {
        y_time = (grid.height() - pos.y) / velocity.y;
    }

    if(x_time >= 0 && x_time < y_time) {
        if(x_time < remaining_time) {
            PARTICLE_TRACER_EVENT(simulation.tracer(), TraceEventType::WallBounce,
                    particle, &simulation, simulation.simulation_time());

            PARTICLE_MOTION_TRACER_EVENT(simulation.tracer(),
                    TraceEventType::MotionParamsUpdated, 
                    particle, &simulation, simulation.simulation_time(), x_time, acceleration, ( 
                [this, &simulation, &particle, x_time, &acceleration]() {
                    simulation.simulate_motion(particle, x_time, acceleration);
                    particle.next_velocity().x = -particle.next_velocity().x;
                }
            ));

            remaining_time -= x_time;
            resolve_border_collision_recursive(simulation, grid, particle, 
                acceleration, remaining_time);
        }
    } else if(y_time >= 0) {
        if(y_time < remaining_time) {
            PARTICLE_TRACER_EVENT(simulation.tracer(), TraceEventType::WallBounce,
                    particle, &simulation, simulation.simulation_time());

            PARTICLE_MOTION_TRACER_EVENT(simulation.tracer(), 
                    TraceEventType::MotionParamsUpdated, 
                    particle, &simulation, simulation.simulation_time(), y_time, acceleration, ( 
                [this, &simulation, &particle, y_time, &acceleration]() {
                    simulation.simulate_motion(particle, y_time, acceleration);
                    particle.next_velocity().y = -particle.next_velocity().y;
                }
            ));

            remaining_time -= y_time;
            resolve_border_collision_recursive(simulation, grid, particle, 
                acceleration, remaining_time);
        }
    }
}
 
