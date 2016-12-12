#ifndef VELOCITYVERLETINTEGRATOR_H_
#define VELOCITYVERLETINTEGRATOR_H_

#include "IMotionIntegrator.h"

class VelocityVerletIntegrator: public IMotionIntegrator {
public:
    VelocityVerletIntegrator() = default;
    virtual ~VelocityVerletIntegrator() = default;

    VelocityVerletIntegrator(const VelocityVerletIntegrator& other) = delete;
    VelocityVerletIntegrator(VelocityVerletIntegrator&& other) noexcept = default;
    VelocityVerletIntegrator& operator =(const VelocityVerletIntegrator& other) = delete;
    VelocityVerletIntegrator& operator =(VelocityVerletIntegrator&& other) noexcept = default;

    virtual void advance_motion(Simulation& simulation, 
            Particle& particle, double dt, const SpatialVector& acceleration, 
            SpatialVector& position, SpatialVector& velocity) const override;
};

#endif
