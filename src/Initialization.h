#ifndef PS_INITIALIZATION_H_
#define PS_INITIALIZATION_H_

#include <random>
#include <functional>

#include "Vector2.h"
#include "Particle.h"
#include "Grid.h"
#include "CommonTypes.h"
#include "InteractionComponent.h"

template <typename Rng>
class PopulationBuilder {
public:
    using RngType = std::remove_reference_t<Rng>;
    PopulationBuilder(Rng& rng, Grid& grid):
        m_rng(rng), m_grid(&grid)
    {
        set_defaults(grid);
    }

    ~PopulationBuilder() = default;

    PopulationBuilder(const PopulationBuilder& other) = delete;
    PopulationBuilder(PopulationBuilder&& other) = default;
    PopulationBuilder& operator =(const PopulationBuilder& other) = delete;
    PopulationBuilder& operator =(PopulationBuilder&& other) = default;

    template <typename Fn>
    PopulationBuilder&& set_position_distribution(Fn&& dist) {
        m_position_dist = std::forward<Fn>(dist);
        return std::move(*this);
    }

    template <typename Fn>
    PopulationBuilder&& set_velocity_distribution(Fn&& dist) {
        m_velocity_dist = std::forward<Fn>(dist);
        return std::move(*this);
    }

    template <typename Fn>
    PopulationBuilder&& set_radius_distribution(Fn&& dist) {
        m_radius_dist = std::forward<Fn>(dist);
        return std::move(*this);
    }

    template <typename Fn>
    PopulationBuilder&& set_mass_distribution(Fn&& dist) {
        m_mass_dist = std::forward<Fn>(dist);
        return std::move(*this);
    }

    PopulationBuilder&& execute(std::size_t num_particles) {
 
        for(std::size_t n = 0; n < num_particles; ++n) {
            Particle p(m_radius_dist(m_rng), m_mass_dist(m_rng), m_position_dist(m_rng));
            p.update_velocity(m_velocity_dist(m_rng));
            p.apply_update();
            m_grid->add(std::move(p));
        }
        m_grid->next_frame();

        return std::move(*this);
    }

private:

    void set_defaults(const Grid& grid);

    std::function<PositionType (Rng&)> m_radius_dist;
    std::function<Vector2<PositionType> (Rng&)> m_position_dist;
    std::function<Vector2<PositionType> (Rng&)> m_velocity_dist;
    std::function<QuantityType (Rng&)> m_mass_dist;
    //std::function<std::unique_ptr<InteractionComponent> (const Particle&)> 
    //    m_interaction_dist = nullptr;

    RngType m_rng;
    Grid* m_grid;
};

template <typename Rng>
PopulationBuilder<Rng> make_population_builder(Rng&& rng, Grid& grid) {
    auto bd = PopulationBuilder<Rng>(std::forward<Rng>(rng), grid);
    return bd;
}

template <typename Rng>
void PopulationBuilder<Rng>::set_defaults(const Grid& grid) {
    m_position_dist = make_vector2_distribution(
        std::uniform_real_distribution<PositionType>(0.0, grid.width()),
        std::uniform_real_distribution<PositionType>(0.0, grid.height()));

    m_velocity_dist = [](Rng& rng) {return Vector2<PositionType>(0.0, 0.0);};

    m_radius_dist = std::uniform_real_distribution<PositionType>(1.0, 2.0);

    m_mass_dist = [](Rng& rng) {return 1.0;};
}

#endif
