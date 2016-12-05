#ifndef BOUNDARYBOUNCERESOLVER_H_
#define BOUNDARYBOUNCERESOLVER_H_

#include "IBoundaryCollisionResolver.h"

#include "CommonTypes.h"

class BoundaryBounceResolver: public IBoundaryCollisionResolver {
public:
    BoundaryBounceResolver() = default;
    virtual ~BoundaryBounceResolver() = default;

    BoundaryBounceResolver(const BoundaryBounceResolver& other) = delete;
    BoundaryBounceResolver(BoundaryBounceResolver&& other) noexcept = default;
    BoundaryBounceResolver& operator =(const BoundaryBounceResolver& other) = delete;
    BoundaryBounceResolver& operator =(BoundaryBounceResolver&& other) noexcept = default;

    virtual void resolve_border_collision(Simulation& simulation, Grid& grid,
            Particle& particle, SpatialVector& acceleration) const override;

private: 
    void resolve_border_collision_recursive(Simulation& simulation, Grid& grid,
            Particle& particle, SpatialVector& acceleration,
            double& remaining_time) const;
};

#endif
