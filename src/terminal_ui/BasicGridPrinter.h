#ifndef TERMINAL_UI_BASICGRIDPRINTER_H_
#define TERMINAL_UI_BASICGRIDPRINTER_H_

#include <ostream>
#include <tuple>

#include "GridPrinter.h"

namespace tui {

class BasicGridPrinter: public IGridPrinter {
public:
    BasicGridPrinter(int width, int height, const Viewport& viewport);
    virtual ~BasicGridPrinter() = default;

    BasicGridPrinter(const BasicGridPrinter& other) = default;
    BasicGridPrinter(BasicGridPrinter&& other) noexcept = default;
    BasicGridPrinter& operator =(const BasicGridPrinter& other) = default;
    BasicGridPrinter& operator =(BasicGridPrinter&& other) noexcept = default;

    virtual void print_grid(std::ostream& stream, const Grid& grid) const override;

    const Viewport& viewport() const {return m_viewport;}
    double cell_width() const {return m_cell_width;}
    double cell_height() const {return m_cell_height;}
    int width() const {return m_width;}
    int height() const {return m_height;}

    std::tuple<int, int> position_to_cell(const SpatialVector& position) const;
    std::size_t position_to_grid_idx(const SpatialVector& position) const;
    bool is_in_grid(const SpatialVector& position) const;

private: 
    Viewport m_viewport;
    int m_width;
    int m_height;
    double m_cell_width;
    double m_cell_height;
};

}

#endif
