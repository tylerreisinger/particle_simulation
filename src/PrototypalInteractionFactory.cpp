#include "PrototypalInteractionFactory.h"


PrototypalInteractionFactory::PrototypalInteractionFactory(
        std::unique_ptr<ClonableParticleInteraction> prototype,
        std::vector<std::string> charge_names):
        m_prototype(std::move(prototype)), m_charge_names(std::move(charge_names)),
        m_charge_mapping(m_charge_names.size()) {

    for(std::size_t i = 0; i < m_charge_names.size(); ++i) {
        m_charge_mapping[i] = i;
    } 
}
 
std::unique_ptr<IParticleInteraction> PrototypalInteractionFactory::build_interaction(
        const Particle& particle) {
    auto interaction = m_prototype->clone();
    interaction->bind_charges(m_charge_mapping);
    return m_prototype->clone(); 
}
 
boost::optional<ChargeIndexType> PrototypalInteractionFactory::get_charge_index(
        const std::string& name) const {
    auto it = std::find(m_charge_names.begin(), m_charge_names.end(), name);
    return it-m_charge_names.begin();
}
 
