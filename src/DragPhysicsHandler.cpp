#include "DragPhysicsHandler.h"

#include "Particle.h"
#include "Simulation.h"


DragPhysicsHandler::DragPhysicsHandler(PositionType drag_coefficient):
    m_drag_coeff(drag_coefficient) {
 
}
 
ForceType DragPhysicsHandler::compute_force(Particle& particle, 
        Simulation& simulation, Grid& grid, const SpatialVector& acceleration) const {
    
    auto dt = simulation.simulation_time().time_delta();
    auto vel = particle.velocity(); 

    auto drag = -vel * (1.0 - std::exp(-m_drag_coeff * dt));

    return drag;
}
 
