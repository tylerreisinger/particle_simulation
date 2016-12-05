#include "ParticlePrinter.h"

#include <vector>

#include "../Grid.h"

namespace tui {

ParticlePrinter::ParticlePrinter(int width, int height, 
        const Viewport& viewport):
    BasicGridPrinter(width, height, viewport)
{}
 
void ParticlePrinter::print_grid(std::ostream& stream, const Grid& grid) const {
    auto format_grid = build_format_grid(grid);

    render(stream, format_grid);     
}
 
std::vector<GridCell> ParticlePrinter::build_format_grid(const Grid& grid) const {
    std::vector<GridCell> format_grid(width()*height());

    for(auto& p : grid) {
        auto& particle = p.second->particle();
        
        if(is_in_grid(particle.position())) {
            auto idx = position_to_grid_idx(particle.position());
            auto& cell = format_grid[idx];
            if(cell.type == CellType::Empty) {
                cell.particle = &particle;
                cell.type = CellType::Particle;
            } else if(cell.type == CellType::Particle) {
                cell.type = CellType::MultipleParticles;
            }
        }
    } 
    return format_grid;
}
 
void ParticlePrinter::render(std::ostream& stream, 
        const std::vector<GridCell>& format_grid) const {
    draw_border_row(stream);
    for(int y = 0; y < height(); ++y) {
        draw_border_cell(stream);
        for(int x = 0; x < width(); ++x) {
            auto cell = format_grid[x + y*width()];
            switch(cell.type) {
            case CellType::Empty:
                draw_empty_cell(stream, cell);
                break;
            case CellType::Particle:
                draw_particle_cell(stream, cell);
                break;
            case CellType::MultipleParticles:
                draw_multi_cell(stream, cell);
                break;
            }
        }
        draw_border_cell(stream);
        stream << "\n";
    } 
    draw_border_row(stream);
    std::cout << std::endl;
}
 
term::TerminalColor ParticlePrinter::foreground_color_for_particle(
        const Particle& particle) const {
    return term::make_terminal_color(term::Color16(term::ColorPalette16::Red)); 
}
 
void ParticlePrinter::draw_empty_cell(std::ostream& stream, const GridCell& cell) const {
    stream << EMPTY_CELL_CHAR;
}
 
void ParticlePrinter::draw_particle_cell(std::ostream& stream, const GridCell& cell) const {
    stream << PARTICLE_CHAR;
}
 
void ParticlePrinter::draw_multi_cell(std::ostream& stream, const GridCell& cell) const {
    stream << MULTI_CELL_CHAR; 
}
 
void ParticlePrinter::draw_border_cell(std::ostream& stream) const {
    if(m_draw_border) {
        term::Terminal::instance().set_background_color(term::ColorPalette16::Gray); 
        stream << " ";
        term::Terminal::instance().reset_formatting();
    }
}
 
void ParticlePrinter::draw_border_row(std::ostream& stream) const {
    for(int i = 0; i < width()+2; ++i) {
        draw_border_cell(stream);
    } 
    stream << "\n";
}
 
}
