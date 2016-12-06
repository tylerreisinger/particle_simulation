#ifndef BOUNDARYBOUNCERESOLVER_H_
#define BOUNDARYBOUNCERESOLVER_H_

#include "IBoundaryCollisionResolver.h"

#include "CommonTypes.h"

class BoundaryBounceResolver: public IBoundaryCollisionResolver {
public:
    BoundaryBounceResolver(PositionType bounce_coefficient = 1.0);
    virtual ~BoundaryBounceResolver() = default;

    BoundaryBounceResolver(const BoundaryBounceResolver& other) = delete;
    BoundaryBounceResolver(BoundaryBounceResolver&& other) noexcept = default;
    BoundaryBounceResolver& operator =(const BoundaryBounceResolver& other) = delete;
    BoundaryBounceResolver& operator =(BoundaryBounceResolver&& other) noexcept = default;

    virtual void resolve_border_collision(Simulation& simulation, Grid& grid,
            Particle& particle, SpatialVector& acceleration) const override;

    PositionType bounce_coefficient() const {return m_bounce_coeff;}
    void set_bounce_coefficient(PositionType value) {m_bounce_coeff = value;}

private: 
    void resolve_border_collision_recursive(Simulation& simulation, Grid& grid,
            Particle& particle, SpatialVector& acceleration,
            double& remaining_time) const;

    PositionType m_bounce_coeff = 1.00;
};

#endif
