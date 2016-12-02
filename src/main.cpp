#include <iostream>
#include <thread>

#include "CommonTypes.h"
#include "Simulation.h"
#include "World.h"
#include "Grid.h"
#include "Particle.h"
#include "Initialization.h"
#include "Vector2.h"

int main(int argc, char** argv) {
    Grid grid(10, 10, 10, 10);

    auto rng = std::mt19937();

    auto bd = make_population_builder(rng, grid)
        .set_position_distribution(
            make_vector2_distribution(
                std::uniform_real_distribution<QuantityType>(0.0, 5.0))
        )
        .set_velocity_distribution(
            make_vector2_distribution(std::uniform_real_distribution<QuantityType>(-1.0, 1.0))
        )
        .execute(1000);

    grid.print_particle_density(std::cout, 0);

    Simulation s(std::move(grid));

    for(int i = 0; i < 2500; ++i) {
        s.do_frame();
        s.get_particles().print_particle_density(std::cout, 0);
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
