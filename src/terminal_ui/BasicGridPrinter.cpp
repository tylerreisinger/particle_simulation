#include "BasicGridPrinter.h"

#include <cmath>
#include <cassert>

#include "../Grid.h"

namespace tui {

BasicGridPrinter::BasicGridPrinter(int width, int height, 
        const Viewport& viewport):
    m_viewport(viewport), m_width(width), m_height(height),
    m_cell_width((viewport.x_max() - viewport.x_min()) / width),
    m_cell_height((viewport.y_max() - viewport.y_min()) / height) {
}
 
void BasicGridPrinter::print_grid(std::ostream& stream, const Grid& grid) const {
 
}
 
std::tuple<int, int> BasicGridPrinter::position_to_cell(const SpatialVector& position) const {
    auto cell_x = std::floor(
            (position.x - viewport().x_min()) / cell_width());
    auto cell_y = std::floor(
            (position.y - viewport().y_min()) / cell_height());
    return std::make_tuple(cell_x, cell_y); 
}
 
std::size_t BasicGridPrinter::position_to_grid_idx(const SpatialVector& position) const {
    int cell_x, cell_y;
    std::tie(cell_x, cell_y) = position_to_cell(position);

    assert(cell_x >= 0);
    assert(cell_y >= 0);
    assert(cell_x < width());
    assert(cell_y < height());   

    return cell_x + cell_y*width(); 
}
 
bool BasicGridPrinter::is_in_grid(const SpatialVector& position) const {
    return viewport().is_within(position);
}
 
}
