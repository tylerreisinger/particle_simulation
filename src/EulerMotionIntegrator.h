#ifndef EULERMOTIONINTEGRATOR_H_
#define EULERMOTIONINTEGRATOR_H_

#include "IMotionIntegrator.h"

class EulerMotionIntegrator: public IMotionIntegrator {
public:
    EulerMotionIntegrator() = default;
    virtual ~EulerMotionIntegrator() = default;

    EulerMotionIntegrator(const EulerMotionIntegrator& other) = delete;
    EulerMotionIntegrator(EulerMotionIntegrator&& other) noexcept = default;
    EulerMotionIntegrator& operator =(const EulerMotionIntegrator& other) = delete;
    EulerMotionIntegrator& operator =(EulerMotionIntegrator&& other) noexcept = default;

    virtual void advance_motion(Simulation& simulation, Particle& particle, double dt,
            const SpatialVector& acceleration, SpatialVector& position,
            SpatialVector& velocity) const override;
    
private:
};

#endif
