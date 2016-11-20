#ifndef PS_WORLD_H_
#define PS_WORLD_H_

#include "CommonTypes.h"
#include "Vector2.h"

template<typename SpatialPartitionContainer>
class World {
public:
    World(PositionType width, PositionType height, PositionType xres, 
            PositionType yres, int num_layers = 1);
    ~World() = default;

    World(const World& other) = delete;
    World(World&& other) noexcept = delete;
    World& operator =(const World& other) = delete;
    World& operator =(World&& other) noexcept = delete;

    SpatialPartitionContainer& get_particles() {
        return m_particles;
    }

    const SpatialPartitionContainer& get_particles() const {
        return m_particles;
    }

private: 
    SpatialPartitionContainer m_particles; 
};

template<typename SpatialPartitionContainer>
inline World<SpatialPartitionContainer>::World(PositionType width, 
        PositionType height, PositionType xres, PositionType yres, int num_layers):
    m_particles(width, height, xres, yres, num_layers)
{ }
 
#endif
