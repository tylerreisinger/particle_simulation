#ifndef IMOTIONINTEGRATOR_H_
#define IMOTIONINTEGRATOR_H_

#include "Vector2.h"
#include "CommonTypes.h"
#include "Particle.h"


class IMotionIntegrator {
public:
    IMotionIntegrator() = default;
    virtual ~IMotionIntegrator() = default;

    virtual void advance_motion(Particle& particle, double dt,
            const SpatialVector& acceleration, SpatialVector& position,
            SpatialVector& velocity) const = 0;

    void advance_motion(Particle& particle, double dt,
            const SpatialVector& acceleration) {
        auto pos = particle.next_position();
        auto vel = particle.next_velocity();
        advance_motion(particle, dt, acceleration, pos, vel);

        particle.update_position(pos);
        particle.update_velocity(vel);
    }
private:
};

#endif
