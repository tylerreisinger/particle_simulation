#ifndef PS_PARTICLE_H_
#define PS_PARTICLE_H_

#include "Vector2.h"

class Particle {
public:
    using QuantityType = float;
    using Vector2t = Vector2<QuantityType>;

    explicit Particle(float radius):
        m_radius(radius) {}

    ~Particle() = default;

    Particle(const Particle& other) = delete;
    Particle(Particle&& other) = default;
    Particle& operator =(const Particle& other) = delete;
    Particle& operator =(Particle&& other) = default;

    const Vector2t& position() const {
        return m_position;
    }

    Vector2t& position() {
        return m_position;
    }

    QuantityType radius() const {
        return m_radius;
    }

    void set_radius(QuantityType radius) {
        m_radius = radius;
    }

private:
    QuantityType m_radius;
    Vector2t m_position = Vector2t(0, 0);
};

#endif
