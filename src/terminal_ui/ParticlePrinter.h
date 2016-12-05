#ifndef TERMINAL_UI_PARTICLEPRINTER_H_
#define TERMINAL_UI_PARTICLEPRINTER_H_

#include <unordered_map>
#include <vector>

#include "Viewport.h"
#include "Terminal.h"
#include "BasicGridPrinter.h"

class Grid;
class Particle;

namespace tui {

enum class CellType {
    Empty,
    Particle,
    MultipleParticles
};

class GridCell {
public:
    GridCell() = default;
    ~GridCell() = default;

    CellType type = CellType::Empty;
    Particle* particle = nullptr;
    term::TerminalColor bg;
};

class ParticlePrinter: public BasicGridPrinter {
public:
    ParticlePrinter(int width, int height, const Viewport& viewport);
    ~ParticlePrinter() = default;

    ParticlePrinter(const ParticlePrinter& other) = delete;
    ParticlePrinter(ParticlePrinter&& other) noexcept = default;
    ParticlePrinter& operator =(const ParticlePrinter& other) = delete;
    ParticlePrinter& operator =(ParticlePrinter&& other) noexcept = default;

    void print_grid(std::ostream& stream, const Grid& grid) const;

private: 
    std::vector<GridCell> build_format_grid(const Grid& grid) const;
    void render(std::ostream& stream, const std::vector<GridCell>& grid) const;
    term::TerminalColor foreground_color_for_particle(const Particle& particle) const;

    void draw_empty_cell(std::ostream& stream, const GridCell& cell) const;
    void draw_particle_cell(std::ostream& stream, const GridCell& cell) const;
    void draw_multi_cell(std::ostream& stream, const GridCell& cell) const;
    void draw_border_cell(std::ostream& stream) const;
    void draw_border_row(std::ostream& stream) const;


    bool m_draw_border = true;

    static constexpr char EMPTY_CELL_CHAR = ' ';
    static constexpr char PARTICLE_CHAR = 'o';
    static constexpr char MULTI_CELL_CHAR = '*';
};


}

#endif
