#include "PrototypalInteractionFactory.h"


PrototypalInteractionFactory::PrototypalInteractionFactory(
        std::unique_ptr<ClonableParticleInteraction> prototype):
    m_prototype(std::move(prototype)) {
}
 
std::unique_ptr<IParticleInteraction> PrototypalInteractionFactory::build_interaction(
        const Particle& particle) {
    return m_prototype->clone(); 
}
 
