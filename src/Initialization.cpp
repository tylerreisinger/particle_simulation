#include "Initialization.h"


void PopulationBuilder::set_defaults(const Grid& grid) {
    m_position_dist = make_vector2_distribution(
            std::uniform_real_distribution<PositionType>(0.0, grid.width()),
            std::uniform_real_distribution<PositionType>(0.0, grid.height()));
}
 
