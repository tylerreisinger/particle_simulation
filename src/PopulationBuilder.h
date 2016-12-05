#ifndef PS_INITIALIZATION_H_
#define PS_INITIALIZATION_H_

#include <random>
#include <functional>
#include <cassert>

#include "Vector2.h"
#include "Particle.h"
#include "Grid.h"
#include "CommonTypes.h"
#include "ParticleInteractionFactory.h"

#include "ParticleParameters.h"

template <typename Rng>
class PopulationBuilder {
public:
    using RngType = std::remove_reference_t<Rng>;
    PopulationBuilder(Rng& rng, Grid& grid):
        m_rng(rng), m_grid(&grid) {
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

    PopulationBuilder&& set_interaction_factory(
            std::unique_ptr<IParticleInteractionFactory> factory) {
        m_interaction_factory = std::move(factory);
        m_charge_dists.clear();
        m_charge_dists.resize(m_interaction_factory->total_charge_count());
        set_charge_defaults();
        return std::move(*this);
    }
    
    PopulationBuilder&& set_charge_distribution(std::size_t idx, 
            std::function<ChargeType (Rng&, const Particle&)> dist) {
        assert(idx < m_charge_dists.size());
        m_charge_dists[idx] = std::move(dist);
        return std::move(*this);
    }

    PopulationBuilder&& set_charge_distribution(std::size_t idx, 
            std::function<ChargeType (Rng&)> dist) {
        return set_charge_distribution(
                [dist(std::move(dist))](Rng& rng, const Particle&) {return dist(rng);});
    }

    template <typename Fn>
    PopulationBuilder&& set_charge_distribution(const std::string& name, Fn&& dist) {
        assert(m_interaction_factory != nullptr);
        auto idx = m_interaction_factory->get_charge_index(name);
        return set_charge_distribution(idx, std::forward<Fn>(dist));
    }
    
    PopulationBuilder&& broadcast_charge_distribution(
            std::function<ChargeType (Rng&)> dist) {
        return broadcast_charge_distribution(
                [dist(std::move(dist))](Rng& rng, const Particle&) {return dist(rng);});
    }
    PopulationBuilder&& broadcast_charge_distribution(
            std::function<ChargeType (Rng&, const Particle&)> dist) {
        for(std::size_t i = 0; i < m_charge_dists.size(); ++i) {
            set_charge_distribution(i, dist);
        }
        return std::move(*this);
    }

    PopulationBuilder&& generate(std::size_t num_particles) {
        for(std::size_t n = 0; n < num_particles; ++n) {
            Particle p(m_radius_dist(m_rng), m_mass_dist(m_rng), 
                    m_position_dist(m_rng), m_velocity_dist(m_rng),
                    num_charges());
            if(m_interaction_factory != nullptr) {
                auto interaction = m_interaction_factory->build_interaction(p);
                p.set_interaction(std::move(interaction));
                for(std::size_t i = 0; i < num_charges(); ++i) {
                    p.set_charge(i, m_charge_dists[i](m_rng, p));
                }
            }
            m_grid->add(std::move(p));
        }
        m_grid->next_frame();

        return std::move(*this);
    }

    PopulationBuilder&& populate(std::initializer_list<ParticleParameters> particles) {
        for(auto& params : particles) {
            auto particle = particle_from_params(params);
            m_grid->add(std::move(particle));
        }
        m_grid->next_frame();
        return std::move(*this);
    }

    std::size_t num_charges() const {return m_interaction_factory->total_charge_count();}


private:
    void set_defaults(const Grid& grid);
    void set_charge_defaults();

    Particle particle_from_params(const ParticleParameters& params) {
        auto mass = params.mass().value_or(m_mass_dist(m_rng));
        auto radius = params.radius().value_or(m_radius_dist(m_rng));
        auto position = params.position().value_or(m_position_dist(m_rng));
        auto velocity = params.velocity().value_or(m_velocity_dist(m_rng));
        Particle p(mass, radius, position, velocity, num_charges());
        if(m_interaction_factory != nullptr) {
            auto interaction = m_interaction_factory->build_interaction(p);
            p.set_interaction(std::move(interaction));
            for(std::size_t i = 0; i < num_charges(); ++i) {
                if(i < params.charges().size()) {
                    p.set_charge(i, params.charges()[i]
                            .value_or(m_charge_dists[i](m_rng, p)));
                } else {
                    p.set_charge(i, m_charge_dists[i](m_rng, p));
                }
            }
        }
        return p;
    }

    std::function<PositionType (Rng&)> m_radius_dist;
    std::function<Vector2<PositionType> (Rng&)> m_position_dist;
    std::function<Vector2<PositionType> (Rng&)> m_velocity_dist;
    std::function<QuantityType (Rng&)> m_mass_dist;
    std::vector<std::function<ChargeType (Rng&, const Particle&)>> m_charge_dists; 

    std::unique_ptr<IParticleInteractionFactory> m_interaction_factory;

    RngType m_rng;
    Grid* m_grid;
};

template <typename Rng>
inline PopulationBuilder<Rng> make_population_builder(Rng&& rng, Grid& grid) {
    auto bd = PopulationBuilder<Rng>(std::forward<Rng>(rng), grid);
    return bd;
}

template <typename Rng>
inline void PopulationBuilder<Rng>::set_defaults(const Grid& grid) {
    m_position_dist = make_vector2_distribution(
        std::uniform_real_distribution<PositionType>(0.0, grid.width()),
        std::uniform_real_distribution<PositionType>(0.0, grid.height()));

    m_velocity_dist = [](Rng& rng) {return Vector2<PositionType>(0.0, 0.0);};

    m_radius_dist = std::uniform_real_distribution<PositionType>(1.0, 2.0);

    m_mass_dist = [](Rng& rng) {return 1.0;};
}

template<typename Rng>
inline void PopulationBuilder<Rng>::set_charge_defaults() {
    for(std::size_t i = 0; i < m_charge_dists.size(); ++i) {
        m_charge_dists[i] = [](Rng&, const Particle&) {return ChargeType(0.0);};
    }
}
 
#endif
