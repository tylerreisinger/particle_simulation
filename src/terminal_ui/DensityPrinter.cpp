#include "DensityPrinter.h"

#include <algorithm>
#include <iomanip>

#include "Terminal.h"
#include "../Grid.h"

namespace tui {

DensityPrinter::DensityPrinter(int width, int height, const Viewport& viewport):
    DensityPrinter(width, height, viewport, DefaultDensityFormatter(), 
        std::make_unique<ParticleDensityBuilder>())
{}
 
DensityPrinter::DensityPrinter(int width, int height, 
        const Viewport& viewport, Formatter formatter, 
        std::unique_ptr<IDensityBuilder> density_builder):
    BasicGridPrinter(width, height, viewport),
    m_formatter(std::move(formatter)), m_density_builder(std::move(density_builder)) {
}
 
void DensityPrinter::print_grid(std::ostream& stream, const Grid& grid) const {
    std::vector<double> density_grid(width()*height());
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
    
    for(int y = 0; y < height(); ++y) {
        for(int x = 0; x < width(); ++x) {
            auto val = density_grid[x + y*width()];
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

        if(printer.is_in_grid(particle.position())) {
            auto idx = printer.position_to_grid_idx(particle.position());
            density_grid[idx] += 1.0;
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

}
