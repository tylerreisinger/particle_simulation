#ifndef PS_PARTICLE_INTERACTION_FACTORY_H_
#define PS_PARTICLE_INTERACTION_FACTORY_H_

#include <memory>

class IParticleInteraction;
class Particle;

class IParticleInteractionFactory {
public:
    IParticleInteractionFactory() = default;
    virtual ~IParticleInteractionFactory() {};

    virtual std::unique_ptr<IParticleInteraction> build_interaction(const Particle& particle) = 0;

};

#endif
