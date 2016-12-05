#include "ParticlePrinter.h"

#include <vector>

#include "../Grid.h"

namespace tui {

ParticlePrinter::ParticlePrinter(int width, int height, 
        const Viewport& viewport):
    BasicGridPrinter(width, height, viewport)
{
    set_default_palette();
}
 
void ParticlePrinter::print_grid(std::ostream& stream, const Grid& grid) const {
    auto format_grid = build_format_grid(grid);

    render(stream, format_grid);     
    term::Terminal::instance().reset_formatting();
}
 
std::vector<GridCell> ParticlePrinter::build_format_grid(const Grid& grid) const {
    std::vector<GridCell> format_grid(width()*height());

    for(auto& p : grid) {
        auto& particle = p.second->particle();
        if(m_particle_colors.find(particle.id()) == m_particle_colors.end()) {
            m_particle_colors.insert(std::make_pair(
                        particle.id(), static_cast<int>(m_particle_colors.size())));
        }
        
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
    auto& color = color_for_particle(*cell.particle);
    term::Terminal::instance().set_foreground_color(color);
    stream << PARTICLE_CHAR;
}
 
void ParticlePrinter::draw_multi_cell(std::ostream& stream, const GridCell& cell) const {
    auto& color = color_for_particle(*cell.particle);
    term::Terminal::instance().set_foreground_color(color);
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
 
void ParticlePrinter::set_default_palette() {
    m_palette = Palette{
        term::Color16(term::ColorPalette16::Red),
        term::Color16(term::ColorPalette16::BrightCyan),
        term::Color16(term::ColorPalette16::BrightBlue),
        term::Color16(term::ColorPalette16::Yellow),
        term::Color16(term::ColorPalette16::BrightGreen),
        term::Color256(100),
        term::Rgb(100, 0, 110)
    }; 
}
 
const term::TerminalColor& ParticlePrinter::color_for_particle(
        const Particle& particle) const {
    auto color_idx = m_particle_colors.at(particle.id());
    auto& color = m_palette[color_idx];
    return color;
}
 
}
