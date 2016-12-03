#ifndef PS_PROTOTYPALINTERACTIONFACTORY_H_
#define PS_PROTOTYPALINTERACTIONFACTORY_H_

#include <memory>
#include <string>

#include "ParticleInteraction.h"
#include "ParticleInteractionFactory.h"

class PrototypalInteractionFactory: public IParticleInteractionFactory {
public:
    PrototypalInteractionFactory(
            std::unique_ptr<ClonableParticleInteraction> interaction,
            std::vector<std::string> charge_names);

    template<typename Derived, typename std::enable_if_t<
        std::is_base_of<ClonableParticleInteraction, Derived>::value, int> = 0>
    PrototypalInteractionFactory(std::unique_ptr<Derived> interaction,
            std::vector<std::string> charge_names):
        m_prototype(std::move(interaction)), 
        m_charge_names(std::move(charge_names))  {
    }

    virtual ~PrototypalInteractionFactory() {};

    PrototypalInteractionFactory(const PrototypalInteractionFactory& other) = delete;
    PrototypalInteractionFactory(PrototypalInteractionFactory&& other) noexcept = default;
    PrototypalInteractionFactory& operator =(const PrototypalInteractionFactory& other) = delete;
    PrototypalInteractionFactory& operator =(PrototypalInteractionFactory&& other) noexcept = default;

    virtual std::unique_ptr<IParticleInteraction> 
        build_interaction(const Particle& particle) override;

    const ClonableParticleInteraction* prototype() const {return m_prototype.get();}

    virtual std::vector<std::string> required_charge_names() const override {
        return m_charge_names;
    }
    virtual std::size_t total_charge_count() const override {
        return m_charge_names.size();
    }
    virtual boost::optional<ChargeIndexType> 
        get_charge_index(const std::string& name) const override;

private:     
    std::unique_ptr<ClonableParticleInteraction> m_prototype;
    std::vector<std::string> m_charge_names;
    std::vector<ChargeIndexType> m_charge_mapping;
};

#endif
