#ifndef BOUNDARYBOUNCERESOLVER_H_
#define BOUNDARYBOUNCERESOLVER_H_

#include "IBoundaryCollisionResolver.h"

#include <tuple>

#include "CommonTypes.h"

struct SubdivideResults {
    double min_time;
    double max_time;
    double end_time;
    SpatialVector final_pos;
    SpatialVector final_vel;
};

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
            double& remaining_time, int recursion_count=0) const;

    bool try_simulate_remainder(Simulation& simulation, Particle& particle,
            Grid& grid, SpatialVector& acceleration, double& remaining_time,
            const SpatialVector& start_pos, const SpatialVector& start_vel) const;

    std::tuple<SpatialVector, SpatialVector> interpolate_to_boundary(
            Simulation& simulation, Particle& particle,
            Grid& grid, double& remaining_time, const SpatialVector& start_pos,
            const SpatialVector& start_vel) const;

    SubdivideResults boundary_collision_subdivide(Simulation& simulation,
            Particle& particle, Grid& grid, SpatialVector& acceleration,
            double& remaining_time, const SpatialVector& position,
            const SpatialVector& velocity) const;

    void resolve_border_collision_recursive2(Simulation& simulation, Grid& grid,
            Particle& particle, SpatialVector& acceleration,
            double& remaining_time) const;

    PositionType m_bounce_coeff = 1.00;
};

#endif
