#include "DensityPrinter.h"

#include <algorithm>
#include <iomanip>

#include "Terminal.h"
#include "../Grid.h"

DensityPrinter::DensityPrinter(int cells_wide, int cells_high, const Viewport& viewport):
    DensityPrinter(cells_wide, cells_high, viewport, DefaultDensityFormatter(), 
        std::make_unique<ParticleDensityBuilder>())
{}
 
DensityPrinter::DensityPrinter(int cells_wide, int cells_high, 
        const Viewport& viewport, Formatter formatter, 
        std::unique_ptr<IDensityBuilder> density_builder):
    m_viewport(viewport), m_cells_wide(cells_wide), m_cells_high(cells_high),
    m_cell_width((viewport.x_max() - viewport.x_min()) / cells_wide),
    m_cell_height((viewport.y_max() - viewport.y_min()) / cells_high), 
    m_formatter(std::move(formatter)), m_density_builder(std::move(density_builder)) {
}
 
void DensityPrinter::draw_state(std::ostream& stream, const Grid& grid) const {
    std::vector<double> density_grid(m_cells_wide*m_cells_high);
    std::fill(density_grid.begin(), density_grid.end(), 0);

    double total_density = 0.0;
    m_density_builder->build_density_grid(density_grid, total_density, grid, *this);
    print_density(stream, density_grid, total_density, grid);

}
void DensityPrinter::print_density(std::ostream& stream, 
        const std::vector<double>& density_grid, double total_density, 
        const Grid& grid) const {
    double max_val = 0;

    for(auto num : density_grid) {
        max_val = std::max(max_val, num);
    }

    int digits = std::floor(std::log10(static_cast<double>(max_val))) + 1;
    
    for(int y = 0; y < m_cells_high; ++y) {
        for(int x = 0; x < m_cells_wide; ++x) {
            auto val = density_grid[x + y*m_cells_wide];
            m_formatter(max_val, val, total_density, density_grid, *this);
            stream << std::setw(digits) << val << " ";
            term::Terminal::instance().reset_formatting();
        }     
        stream << "\n";
    }     
}
 
void ParticleDensityBuilder::build_density_grid(std::vector<double>& density_grid, 
        double& total_density, const Grid& grid, const DensityPrinter& printer) {
    total_density = 0;

    for(auto& p : grid) {
        total_density += 1;
        auto& particle = p.second->particle();
        auto cell_x = std::floor(
                (particle.position().x - printer.viewport().x_min()) 
                / printer.cell_width());
        auto cell_y = std::floor(
                (particle.position().y -printer.viewport().y_min()) 
                / printer.cell_height());

        if(cell_x >= 0 && cell_x < printer.cells_wide() 
                && cell_y >= 0 && cell_y < printer.cells_high()) {
            density_grid[cell_x + cell_y*printer.cells_wide()] += 1.0;
        }
    }
}
 
void DefaultDensityFormatter::operator()(double max_val, double cell_val, 
        double total_sum, const std::vector<double>& grid, const DensityPrinter& printer) {
    if(cell_val != 0) {
        double denom = total_sum * 0.20;
        double scale = (cell_val - 1) / denom;
        scale = std::min(scale, 1.0);

        int color_idx = std::floor(std::sqrt(scale) * 5.99);
        term::Terminal::instance().set_foreground_color(term::Color256(52 + (5-color_idx)));
    }
}
 
