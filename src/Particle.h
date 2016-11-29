#ifndef PS_PARTICLE_H_
#define PS_PARTICLE_H_

#include <memory>

#include "Vector2.h"
#include "DoubleBuffered.h"
#include "InteractionManager.h"
#include "CommonTypes.h"

class Particle {
public:
    using Vector2t = Vector2<QuantityType>;

    Particle(QuantityType radius, QuantityType mass, 
            const Vector2t& position=Vector2t::zero()):
        m_position(position), m_radius(radius), m_mass(mass), m_id(m_next_id++) 
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

    const Vector2t& velocity() const {
        return m_velocity.get(); 
    }

    const Vector2t& next_velocity() const {
        return m_velocity.get_updated_value(); 
    }

    QuantityType radius() const {
        return m_radius;
    }

    void set_radius(QuantityType radius) {
        m_radius = radius;
    }

    QuantityType mass() const {
        return m_mass;
    }

    int id() const {
        return m_id;
    }

private:
    DoubleBuffered<Vector2t> m_position = Vector2t(0, 0);
    DoubleBuffered<Vector2t> m_velocity = Vector2t(0, 0);
    QuantityType m_radius;
    QuantityType m_mass;
    int m_id;

    std::shared_ptr<InteractionManager> m_interaction;

    static int m_next_id;
};

#endif
