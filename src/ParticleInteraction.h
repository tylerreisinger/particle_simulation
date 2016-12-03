#ifndef PS_PARTICLEINTEREACTION_H_
#define PS_PARTICLEINTEREACTION_H_

#include <memory>

#include "CommonTypes.h"

class Particle;

class IParticleInteraction {
public:
    IParticleInteraction() = default;
    virtual ~IParticleInteraction() {};

    virtual ForceType compute_force(const Particle& target, const Particle& src) const = 0;


};

class ClonableParticleInteraction: public IParticleInteraction {
public:
    ClonableParticleInteraction() = default;
    virtual ~ClonableParticleInteraction() {};

    virtual std::unique_ptr<ClonableParticleInteraction> clone() const = 0;
};

#endif
