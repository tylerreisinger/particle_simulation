#ifndef PS_INITIALIZATION_H_
#define PS_INITIALIZATION_H_

#include <random>

#include "Vector2.h"
#include "Particle.h"
#include "Grid.h"

template<typename Rng,
    typename Dist1,
    typename Dist2,
    typename Dist3
    >
void populate_grid(Rng& rng, std::size_t num_particles, Grid& grid,
        Dist1&& position_distribution, 
        Dist2&& velocity_distribution,
        Dist3&& radius_distribution 
        ) {
    for(std::size_t i = 0; i < num_particles; ++i) {
        Particle p(radius_distribution(rng));
        p.update_position(position_distribution(rng));
        p.update_velocity(velocity_distribution(rng));

        p.apply_update();

        grid.add(p);
    }

    grid.next_frame();
}

template<typename T>
class GridBuilderValue {
public:
    GridBuilderValue() = default;
    GridBuilderValue(const T& value):
        value(value) {}

    constexpr bool has_value() const {return true;}
    T value;
};

template<>
class GridBuilderValue<void> {
public:
    GridBuilderValue() = default;
    constexpr bool has_value() const {return false;}
};

template<typename PDist, typename VDist, typename RDist>
class GridBuilder {
public:
    GridBuilder() = default;

    GridBuilder(const PDist& position_distribution,
            const VDist& velocity_distribution,
            const RDist& radius_distribution):
        m_position_dist(position_distribution),
        m_velocity_dist(velocity_distribution),
        m_radius_dist(radius_distribution)
    {}

    ~GridBuilder() = default;

    GridBuilder(const GridBuilder& other) = default;
    GridBuilder(GridBuilder&& other) noexcept = default;
    GridBuilder& operator =(const GridBuilder& other) = default;
    GridBuilder& operator =(GridBuilder&& other) noexcept = default;

    template<typename T>
    GridBuilder<T, VDist, RDist> set_position_distribution(T&& dist) {
        return GridBuilder<std::remove_cv_t<T>, VDist, RDist>(
            std::forward<std::remove_cv_t<T>>(dist), std::move(m_velocity_dist.value), 
            std::move(m_radius_dist.value));
    }

    template<typename T>
    GridBuilder<PDist, T, RDist> set_velocity_distribution(T&& dist) {
        return GridBuilder<PDist, T, RDist>(
            std::move(m_position_dist.value), std::forward<T>(dist), 
            std::move(m_radius_dist.value));
    }
    
    template<typename T>
    GridBuilder<PDist, VDist, T> set_radius_distribution(T&& dist) {
        return GridBuilder<PDist, VDist, T>(
            std::move(m_position_dist.value), std::move(m_velocity_dist.value), 
            std::forward<T>(dist));
    }

    template<typename Rng>
    void execute(Rng& rng, std::size_t num_particles, Grid& grid) {
        populate_grid(rng, num_particles, grid, m_position_dist.value,
            m_velocity_dist.value, m_radius_dist.value);    
    }


private:
    GridBuilderValue<PDist> m_position_dist;
    GridBuilderValue<VDist> m_velocity_dist;
    GridBuilderValue<RDist> m_radius_dist;
};

template<typename PDist, typename VDist, typename RDist>
auto make_population_builder(const PDist& pdist, const VDist& vdist, const RDist& rdist) {
    return GridBuilder<PDist, VDist, RDist>(pdist, vdist, rdist);
}

auto make_population_builder(const Grid& grid) {
    return make_population_builder(
        make_vector2_distribution(
            std::uniform_real_distribution<PositionType>(0.0, grid.width()),
            std::uniform_real_distribution<PositionType>(0.0, grid.height())
            ),
        make_vector2_distribution(
            std::uniform_real_distribution<PositionType>(0.0, 0.0)
            ),
        std::uniform_real_distribution<PositionType>(1.0, 2.0)
    );
}

#endif
