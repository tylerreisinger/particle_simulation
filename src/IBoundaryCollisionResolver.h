#ifndef IBOUNDARYCOLLISIONRESOLVER_H_
#define IBOUNDARYCOLLISIONRESOLVER_H_

#include "CommonTypes.h"
#include "Vector2.h"

class Particle;
class Simulation;
class Grid;

class IBoundaryCollisionResolver {
public:
    IBoundaryCollisionResolver() = default;
    virtual ~IBoundaryCollisionResolver() {};

    virtual void resolve_border_collision(Simulation& simulation, Grid& grid,
            Particle& particle, SpatialVector& acceleration) const = 0;
};

#endif
