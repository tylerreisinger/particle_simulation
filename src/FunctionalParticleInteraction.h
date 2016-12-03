#ifndef FUNCTIONALPARTICLEINTERACTION_H_
#define FUNCTIONALPARTICLEINTERACTION_H_

#include <utility>
#include <array>

#include "ParticleInteraction.h"
#include "Vector2.h"

template <typename Fn, typename Enable = void>
class FunctionalParticleInteractionHelper;

template <typename Fn, std::size_t NumCharges>
class FunctionalParticleInteraction: public FunctionalParticleInteractionHelper<Fn> {
public:
    FunctionalParticleInteraction(Fn&& force_function);
    virtual ~FunctionalParticleInteraction() = default;

    FunctionalParticleInteraction(const FunctionalParticleInteraction& other) = default;
    FunctionalParticleInteraction(FunctionalParticleInteraction&& other) noexcept = default;
    FunctionalParticleInteraction& operator =(const FunctionalParticleInteraction& other) = default;
    FunctionalParticleInteraction& operator =(FunctionalParticleInteraction&& other) noexcept = default;

    virtual ForceType compute_force(const Particle& target, const Particle& src) const override;    

    virtual std::unique_ptr<ClonableParticleInteraction> clone() const override {
        return std::unique_ptr<ClonableParticleInteraction>(
            new FunctionalParticleInteraction<Fn, NumCharges>(*this));
    }

private:
    Fn m_fn;
    std::array<ChargeType, NumCharges> m_charges;
};

template <typename Fn>
class FunctionalParticleInteractionHelper<Fn, std::enable_if_t<
    std::is_copy_constructible<Fn>::value>>: public ClonableParticleInteraction {

};

template <typename Fn>
class FunctionalParticleInteractionHelper<Fn, std::enable_if_t<
    !std::is_copy_constructible<Fn>::value>>: public IParticleInteraction {
};


template <std::size_t NumCharges, typename Fn>
std::unique_ptr<FunctionalParticleInteraction<Fn, NumCharges>> 
    make_functional_particle_interaction(
        Fn&& force_function) {
    return std::make_unique<FunctionalParticleInteraction<Fn, NumCharges>>
        (std::forward<Fn>(force_function));
}

template <typename Fn, std::size_t NumCharges>
inline FunctionalParticleInteraction<Fn, NumCharges>::FunctionalParticleInteraction(
        Fn&& force_function):
    m_fn(std::forward<Fn>(force_function)) { 
}
 
template <typename Fn, std::size_t NumCharges>
inline ForceType FunctionalParticleInteraction<Fn, NumCharges>::compute_force(
        const Particle& target, const Particle& src) const {
    return m_fn(m_charges, target, src); 
}
 
#endif
