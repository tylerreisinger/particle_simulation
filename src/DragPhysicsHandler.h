#ifndef DRAGPHYSICSHANDLER_H_
#define DRAGPHYSICSHANDLER_H_

#include "IWorldPhysicsHandler.h"

class DragPhysicsHandler: public IWorldPhysicsHandler {
public:
    DragPhysicsHandler(PositionType drag_coefficient = 0.001);
    virtual ~DragPhysicsHandler() = default;

    DragPhysicsHandler(const DragPhysicsHandler& other) = delete;
    DragPhysicsHandler(DragPhysicsHandler&& other) noexcept = default;
    DragPhysicsHandler& operator =(const DragPhysicsHandler& other) = delete;
    DragPhysicsHandler& operator =(DragPhysicsHandler&& other) noexcept = default;

    virtual ForceType compute_force(Particle& particle, Simulation& simulation,
            Grid& grid) const override;

    PositionType drag_coefficient() const {return m_drag_coeff;}
    void set_drag_coefficient(PositionType value) {m_drag_coeff = value;}
    
private:
    PositionType exact_force_multiplier(double dt) const;
    PositionType taylor_series_force_multiplier(double dt) const;

    PositionType m_drag_coeff = 0.001;    
};

#endif
