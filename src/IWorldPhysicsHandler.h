#ifndef IWORLDPHYSICSHANDLER_H_
#define IWORLDPHYSICSHANDLER_H_

#include "Vector2.h"
#include "CommonTypes.h"

class Particle;
class Simulation;
class Grid;

class IWorldPhysicsHandler {
public:
    IWorldPhysicsHandler() = default;
    virtual ~IWorldPhysicsHandler() = default;

    virtual ForceType compute_force(Particle& particle, Simulation& simulation,
            Grid& grid, const SpatialVector& acceleration) const = 0;

private:
};

#endif
