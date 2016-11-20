#ifndef PS_PARTICLE_H_
#define PS_PARTICLE_H_

#include "Vector2.h"
#include "DoubleBuffered.h"

class Particle {
public:
    using QuantityType = float;
    using Vector2t = Vector2<QuantityType>;

    Particle(float radius, const Vector2t& position=Vector2t::zero()):
        m_radius(radius), m_position(position), m_id(m_next_id++) 
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
    }

    const Vector2t& position() const {
        return m_position.get();
    }

    const Vector2t& velocity() const {
        return m_velocity.get(); 
    }

    QuantityType radius() const {
        return m_radius;
    }

    void set_radius(QuantityType radius) {
        m_radius = radius;
    }

    int id() const {
        return m_id;
    }

private:
    QuantityType m_radius;
    DoubleBuffered<Vector2t> m_position = Vector2t(0, 0);
    DoubleBuffered<Vector2t> m_velocity = Vector2t(0, 0);
    int m_id;

    static int m_next_id;
};

#endif
