#include <iostream>

#include "World.h"
#include "Grid.h"
#include "Particle.h"
#include "Initialization.h"
#include "Vector2.h"

int main(int argc, char** argv) {
    Grid grid(10, 10, 10, 10);

    grid.add(Particle(2.0));
    grid.add(Particle(2.0, {1.6, 2.4}));

    grid.print_particle_density(std::cout, 0);

    grid.next_frame();

    grid.print_particle_density(std::cout, 0);

    auto rng = std::mt19937();

    make_population_builder(grid)
        .set_position_distribution(make_vector2_distribution(
            std::uniform_real_distribution<float>(0.0, 5.0)))
        .execute(rng, 50, grid);

    grid.print_particle_density(std::cout, 0);

    return 0;
}
