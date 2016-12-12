#include "DragPhysicsHandler.h"

#include "Particle.h"
#include "Simulation.h"


DragPhysicsHandler::DragPhysicsHandler(PositionType drag_coefficient):
    m_drag_coeff(drag_coefficient) {
 
}
 
ForceType DragPhysicsHandler::compute_force(Particle& particle, 
        Simulation& simulation, Grid& grid) const {
    
    auto dt = simulation.simulation_time().time_delta();
    auto vel = particle.velocity(); 

    auto drag = -vel * taylor_series_force_multiplier(dt);

    return drag;
}
 
PositionType DragPhysicsHandler::exact_force_multiplier(double dt) const {
    //   dv
    //  --- = - m_drag_coeff * v 
    //   dt
    return (1.0 - std::exp(-m_drag_coeff * dt)); 
}
 
PositionType DragPhysicsHandler::taylor_series_force_multiplier(double dt) const {
    //Taylor series approximation to 1.0 - e^(-a*x)
    // = ax - (ax)^2/2 + (ax)^3/6 - (ax)^4/24 + O(x^5)
    // Third order error < 1% at a=0.5 until about dt=1.
    // At low a, the approximation is very good
    auto a_t = m_drag_coeff*dt;

    return a_t * (PositionType(1.0) + a_t * (PositionType(-0.5)
            + PositionType(0.1666666666)*a_t));
}
 
