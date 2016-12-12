#include "VelocityVerletIntegrator.h"

#include "Simulation.h"

void VelocityVerletIntegrator::advance_motion(Simulation& simulation, 
        Particle& particle, double dt, const SpatialVector& acceleration, 
        SpatialVector& position, SpatialVector& velocity) const {

    auto v_half = velocity + PositionType(0.5) * acceleration * dt; 
    auto pos = position + v_half * dt;
    position = pos;

    auto a_end = simulation.compute_acceleration(particle, position, velocity);
    auto v = v_half + PositionType(0.5)*a_end*dt;
    velocity = v;
}
 
