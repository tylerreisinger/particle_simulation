#ifndef PS_PARTICLE_INTERACTION_FACTORY_H_
#define PS_PARTICLE_INTERACTION_FACTORY_H_

#include <memory>
#include <vector>
#include <string>
#include <boost/optional.hpp>

#include "CommonTypes.h"

class IParticleInteraction;
class Particle;

class IParticleInteractionFactory {
public:
    IParticleInteractionFactory() = default;
    virtual ~IParticleInteractionFactory() {};

    virtual std::unique_ptr<IParticleInteraction> build_interaction(const Particle& particle) = 0;

    virtual std::vector<std::string> required_charge_names() const = 0;
    virtual std::size_t total_charge_count() const = 0;

    virtual boost::optional<ChargeIndexType> 
        get_charge_index(const std::string& name) const = 0;
};

#endif
