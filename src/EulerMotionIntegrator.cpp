#include "EulerMotionIntegrator.h"

#include "Particle.h"

void EulerMotionIntegrator::advance_motion(Simulation& simulation, Particle& particle, 
        double dt, const SpatialVector& acceleration, SpatialVector& position, 
        SpatialVector& velocity) const {

    auto next_vel = velocity + acceleration*dt; 
    auto next_pos = position + velocity*dt;

    position = next_pos;
    velocity = next_vel;
}
