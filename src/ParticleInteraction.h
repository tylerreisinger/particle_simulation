#ifndef PS_PARTICLEINTEREACTION_H_
#define PS_PARTICLEINTEREACTION_H_

#include <vector>
#include <unordered_map>
#include <memory>

#include "InteractionComponent.h"
#include "CommonTypes.h"
#include "Particle.h"

class ParticleInteraction {
public:
    ParticleInteraction(const Particle* particle = nullptr);
    ~ParticleInteraction() = default;

    ParticleInteraction(const ParticleInteraction& other) = delete;
    ParticleInteraction(ParticleInteraction&& other) noexcept = default;
    ParticleInteraction& operator =(const ParticleInteraction& other) = delete;
    ParticleInteraction& operator =(ParticleInteraction&& other) noexcept = default;

    void add_interaction_component(std::unique_ptr<InteractionComponent> component) {
    }

    ForceType compute_force(const Particle& target, const Particle& source) {
        ForceType force = 0;
        for(std::size_t i = 0; i < m_components.size(); ++i) {
            force += m_components[i]->compute_force(target, source);
        }
        return force;
    }

    ParticleInteraction clone(const Particle* parent) const {
        ParticleInteraction clone(parent);

        for(auto& component : m_components) {
            clone.add_interaction_component(component->clone());
        }

        return clone;
    }

private: 
    std::vector<ChargeType> m_charges;
    std::vector<std::unique_ptr<InteractionComponent>> m_components;
};

#endif
