#ifndef PS_PROTOTYPALINTERACTIONFACTORY_H_
#define PS_PROTOTYPALINTERACTIONFACTORY_H_

#include <memory>

#include "ParticleInteraction.h"
#include "ParticleInteractionFactory.h"

class PrototypalInteractionFactory: public IParticleInteractionFactory {
public:
    explicit PrototypalInteractionFactory(
            std::unique_ptr<ClonableParticleInteraction> interaction);

    template<typename Derived, typename std::enable_if_t<
        std::is_base_of<ClonableParticleInteraction, Derived>::value, int> = 0>
    PrototypalInteractionFactory(std::unique_ptr<Derived> interaction):
        m_prototype(std::move(interaction)) {
    }

    virtual ~PrototypalInteractionFactory() {};

    PrototypalInteractionFactory(const PrototypalInteractionFactory& other) = delete;
    PrototypalInteractionFactory(PrototypalInteractionFactory&& other) noexcept = default;
    PrototypalInteractionFactory& operator =(const PrototypalInteractionFactory& other) = delete;
    PrototypalInteractionFactory& operator =(PrototypalInteractionFactory&& other) noexcept = default;

    virtual std::unique_ptr<IParticleInteraction> 
        build_interaction(const Particle& particle) override;

    const ClonableParticleInteraction* prototype() const {return m_prototype.get();}

private:     
    std::unique_ptr<ClonableParticleInteraction> m_prototype;
};

#endif
