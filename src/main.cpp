#include <iostream>
#include <thread>
#include <cmath>

#include "CommonTypes.h"
#include "Simulation.h"
#include "World.h"
#include "Grid.h"
#include "Particle.h"
#include "Initialization.h"
#include "Vector2.h"

#include "PrototypalInteractionFactory.h"
#include "FunctionalParticleInteraction.h"

int main(int argc, char** argv) {
    Grid grid(10, 10, 10, 10);

    auto rng = std::mt19937();

    auto interaction_prototype = make_functional_particle_interaction<1>([]
                    (const std::array<ChargeType, 1>& charges, const Particle& target, 
                            const Particle& src) {
                        auto dist = (target.position() - src.position()).magnitude_squared();
                        dist = std::max(dist, QuantityType(0.05));
                        return SpatialVector(0.1, 0.1) / dist;
                    });

    auto interaction_factory = std::make_unique<PrototypalInteractionFactory>(
        std::move(interaction_prototype));

    auto bd = make_population_builder(rng, grid)
        .set_position_distribution(
            make_vector2_distribution(
                std::uniform_real_distribution<QuantityType>(0.0, 5.0))
        )
        .set_velocity_distribution(
            make_vector2_distribution(
                std::uniform_real_distribution<QuantityType>(-1.0, 1.0))
        )
        .set_interaction_factory(
            std::move(interaction_factory)
        )
        .execute(5);

    grid.print_particle_density(std::cout, 0);

    Simulation s(std::move(grid));

    for(int i = 0; i < 5000; ++i) {
        s.do_frame();
        s.get_particles().print_particle_density(std::cout, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 0;
}
