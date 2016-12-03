#ifndef PS_PARTICLE_H_
#define PS_PARTICLE_H_

#include <memory>
#include <cassert>

#include "Vector2.h"
#include "DoubleBuffered.h"
#include "CommonTypes.h"
#include "ParticleInteraction.h"

class Particle {
public:
    using Vector2t = Vector2<QuantityType>;

    Particle(QuantityType radius, QuantityType mass, 
            const Vector2t& position=Vector2t::zero(),
            std::unique_ptr<IParticleInteraction> interaction = nullptr):
        m_position(position), m_radius(radius), m_mass(mass), m_id(m_next_id++),
        m_interaction(std::move(interaction))
    {}

    ~Particle() = default;

    Particle(const Particle& other) = default;
    Particle(Particle&& other) noexcept = default;
    Particle& operator =(const Particle& other) = default;
    Particle& operator =(Particle&& other) noexcept = default;

    void set_position(const Vector2t& position) {
        m_position.broadcast(position);
    }

    void update_position(const Vector2t& position) {
        m_position.set(position);
    }

    void update_velocity(const Vector2t& velocity) {
        m_velocity.set(velocity);
    }

    void apply_update() {
        m_position.apply_update();
        m_velocity.apply_update();
    }

    const Vector2t& position() const {
        return m_position.get();
    }
    const Vector2t& next_position() const {
        return m_position.get_updated_value();
    }
    Vector2t& next_position() {
        return m_position.get_updated_value();
    }
    void reset_position() {
        m_position.reset();
    }
    const Vector2t& velocity() const {
        return m_velocity.get(); 
    }
    const Vector2t& next_velocity() const {
        return m_velocity.get_updated_value(); 
    }
    Vector2t& next_velocity() {
        return m_velocity.get_updated_value(); 
    }
    void reset_velocity() {
        m_velocity.reset();
    }

    QuantityType radius() const {
        return m_radius;
    }

    void set_radius(QuantityType radius) {
        m_radius = radius;
    }

    const IParticleInteraction& interaction() const {return *m_interaction;}
    void set_interaction(std::unique_ptr<IParticleInteraction> interaction) {
        m_interaction = std::move(interaction);
    }

    QuantityType mass() const {
        return m_mass;
    }

    int id() const {
        return m_id;
    }

    ForceType compute_force(const Particle& target) const {
        assert(m_interaction != nullptr);
        return m_interaction->compute_force(target, *this);
    }

private:
    DoubleBuffered<Vector2t> m_position = Vector2t(0, 0);
    DoubleBuffered<Vector2t> m_velocity = Vector2t(0, 0);
    QuantityType m_radius;
    QuantityType m_mass;
    int m_id;

    std::unique_ptr<IParticleInteraction> m_interaction;

    static int m_next_id;
};

#endif
