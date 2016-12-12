#ifndef FUNCTIONALPARTICLEINTERACTION_H_
#define FUNCTIONALPARTICLEINTERACTION_H_

#include <utility>
#include <vector>

#include "ParticleInteraction.h"
#include "Vector2.h"
#include "CommonTypes.h"
#include "Particle.h"

template <typename Fn, typename Enable = void>
class FunctionalParticleInteractionHelper;

template <typename Fn>
class FunctionalParticleInteraction: public FunctionalParticleInteractionHelper<Fn> {
public:
    FunctionalParticleInteraction(Fn&& force_function);
    FunctionalParticleInteraction(std::vector<ChargeIndexType> charges, 
            Fn&& force_function);

    virtual ~FunctionalParticleInteraction() = default;

    FunctionalParticleInteraction(const FunctionalParticleInteraction& other) = default;
    FunctionalParticleInteraction(FunctionalParticleInteraction&& other) noexcept = default;
    FunctionalParticleInteraction& operator =(const FunctionalParticleInteraction& other) = default;
    FunctionalParticleInteraction& operator =(FunctionalParticleInteraction&& other) noexcept = default;

    virtual ForceType compute_force(const Particle& target, 
            const Particle& src) const override;    
    virtual ForceType compute_force(const Particle& target, const Particle& src,
            const SpatialVector& src_position, 
            const SpatialVector& src_velocity) const override;

    virtual std::vector<ChargeIndexType> required_charges() const override {
        return m_charge_indices;
    }
    virtual void bind_charges(std::vector<ChargeIndexType> charge_indices) override {
        m_charge_indices = std::move(charge_indices);
    }

    virtual std::unique_ptr<ClonableParticleInteraction> clone() const override {
        return std::unique_ptr<ClonableParticleInteraction>(
            new FunctionalParticleInteraction<Fn>(*this));
    }

private:
    Fn m_fn;
    std::vector<ChargeIndexType> m_charge_indices;
};

template <typename Fn>
class FunctionalParticleInteractionHelper<Fn, std::enable_if_t<
    std::is_copy_constructible<Fn>::value>>: public ClonableParticleInteraction {

};

template <typename Fn>
class FunctionalParticleInteractionHelper<Fn, std::enable_if_t<
    !std::is_copy_constructible<Fn>::value>>: public IParticleInteraction {
};


template <typename Fn>
std::unique_ptr<FunctionalParticleInteraction<Fn>> 
    make_functional_particle_interaction(
        Fn&& force_function) {
    return std::make_unique<FunctionalParticleInteraction<Fn>>
        (std::forward<Fn>(force_function));
}

template<typename Fn>
inline FunctionalParticleInteraction<Fn>::FunctionalParticleInteraction(
        Fn&& force_function):
    m_fn(std::forward<Fn>(force_function)) {
}
 
template <typename Fn>
inline FunctionalParticleInteraction<Fn>::FunctionalParticleInteraction(
        std::vector<ChargeIndexType> charge_indices, Fn&& force_function):
    m_fn(std::forward<Fn>(force_function)), 
    m_charge_indices(std::move(charge_indices)) { 
}
 
template <typename Fn>
inline ForceType FunctionalParticleInteraction<Fn>::compute_force(
        const Particle& target, const Particle& src) const {
    return m_fn(target, src, src.next_position(), src.next_velocity()); 
}
 
template<typename Fn>
inline ForceType FunctionalParticleInteraction<Fn>::compute_force(const Particle& target, 
        const Particle& src, const SpatialVector& position, 
        const SpatialVector& velocity) const {
    return m_fn(target, src, position, velocity); 
}
 
#endif
