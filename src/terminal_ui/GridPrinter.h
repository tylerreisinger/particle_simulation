#ifndef TERMINAL_UI_GRIDPRINTER_H_
#define TERMINAL_UI_GRIDPRINTER_H_

#include <ostream>

#include "Viewport.h"

class Grid;

class IGridPrinter {
public:
    IGridPrinter() = default;
    virtual ~IGridPrinter() = default;

    virtual void print_grid(std::ostream& stream, const Grid& grid) const = 0;
};

#endif
