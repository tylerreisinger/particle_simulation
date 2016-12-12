#ifndef IMOTIONINTEGRATOR_H_
#define IMOTIONINTEGRATOR_H_

#include "Vector2.h"
#include "CommonTypes.h"
#include "Particle.h"

class Simulation;

class IMotionIntegrator {
public:
    IMotionIntegrator() = default;
    virtual ~IMotionIntegrator() = default;

    virtual void advance_motion(Simulation& simulation, Particle& particle, double dt,
            const SpatialVector& acceleration, SpatialVector& position,
            SpatialVector& velocity) const = 0;

private:
};

#endif
