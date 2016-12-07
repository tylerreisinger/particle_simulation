#include <iostream>
#include <thread>
#include <cmath>

#include "CommonTypes.h"
#include "Simulation.h"
#include "World.h"
#include "Grid.h"
#include "Particle.h"
#include "PopulationBuilder.h"
#include "Vector2.h"

#include "PrototypalInteractionFactory.h"
#include "FunctionalParticleInteraction.h"
#include "SimulationRunner.h"

#include "terminal_ui/Terminal.h"
#include "terminal_ui/DensityPrinter.h"
#include "terminal_ui/ParticlePrinter.h"

int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);
    std::setvbuf(stdout, nullptr, _IOFBF, BUFSIZ);

    Grid grid(10, 10, 10, 10);

    auto rng = std::mt19937();

    auto force_fn = 
        [](const Particle& target, const Particle& src) {
            auto r = (target.position() - src.position());
            auto dist = r.magnitude_squared();
            dist = std::max(dist, target.radius()*target.radius());
            return (src.get_charge(0)*target.get_charge(0)) 
                * PositionType(1.0 / dist) * r.to_unit();
        };

    auto interaction_prototype = make_functional_particle_interaction(std::move(force_fn));

    auto interaction_factory = std::make_unique<PrototypalInteractionFactory>(
        std::move(interaction_prototype), std::vector<std::string>{"q1", "q2"});

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
        .set_radius_distribution(
            std::uniform_real_distribution<QuantityType>(0.2, 0.5)
        )
        .broadcast_charge_distribution(
            std::uniform_real_distribution<QuantityType>(0.0, 1.0)
        )
        .generate(10);

    grid.print_particle_density(std::cout, 0);

    auto output = tui::DensityPrinter(10, 10, {0, 0, 10, 10});
    auto output2 = tui::ParticlePrinter(20, 20, {0, 0, 10, 10});

    auto s = std::make_unique<Simulation>(std::move(grid), 0.05);
    SimulationRunner runner(std::move(s));
    runner.set_stopping_time(50.0);
    runner.set_delay(std::chrono::milliseconds(25));

    auto handler = runner.on_frame_end(
        [&output, &output2](Simulation& sim, SimulationRunner& runner) {
            output.print_grid(std::cout, sim.get_particles());
            output2.print_grid(std::cout, sim.get_particles());
        });

    runner.run();

    return 0;
}
