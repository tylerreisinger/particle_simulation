#ifndef PS_PARTICLEINTEREACTION_H_
#define PS_PARTICLEINTEREACTION_H_

#include "CommonTypes.h"

class Particle;

class IParticleInteraction {
public:
    IParticleInteraction() = default;
    virtual ~IParticleInteraction() {};

    virtual ForceType compute_force(const Particle& other) = 0;
};

#endif
