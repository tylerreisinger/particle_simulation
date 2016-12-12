#ifndef PS_PARTICLEINTEREACTION_H_
#define PS_PARTICLEINTEREACTION_H_

#include <memory>
#include <vector>

#include "CommonTypes.h"

class Particle;

class IParticleInteraction {
public:
    IParticleInteraction() = default;
    virtual ~IParticleInteraction() {};

    virtual ForceType compute_force(const Particle& target, const Particle& src) const = 0;
    virtual ForceType compute_force(const Particle& target, const Particle& src,
            const SpatialVector& position, const SpatialVector& velocity) const = 0;

    virtual std::vector<ChargeIndexType> required_charges() const {return {};}
    virtual void bind_charges(std::vector<ChargeIndexType> charge_indices) = 0;
};

class ClonableParticleInteraction: public IParticleInteraction {
public:
    ClonableParticleInteraction() = default;
    virtual ~ClonableParticleInteraction() {};

    virtual std::unique_ptr<ClonableParticleInteraction> clone() const = 0;
};

#endif
