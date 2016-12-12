#ifndef SEMIIMPLICITEULERINTEGRATOR_H_
#define SEMIIMPLICITEULERINTEGRATOR_H_

#include "IMotionIntegrator.h"

class SemiImplicitEulerIntegrator: public IMotionIntegrator {
public:
    SemiImplicitEulerIntegrator() = default;
    ~SemiImplicitEulerIntegrator() = default;

    SemiImplicitEulerIntegrator(const SemiImplicitEulerIntegrator& other) = delete;
    SemiImplicitEulerIntegrator(SemiImplicitEulerIntegrator&& other) noexcept = default;
    SemiImplicitEulerIntegrator& operator =(const SemiImplicitEulerIntegrator& other) = delete;
    SemiImplicitEulerIntegrator& operator =(SemiImplicitEulerIntegrator&& other) noexcept = default;

    virtual void advance_motion(Simulation& simulation, Particle& particle, double dt,
            const SpatialVector& acceleration, SpatialVector& position,
            SpatialVector& velocity) const override;

};

#endif
