#ifndef TERMINAL_UI_DENSITY_PRINTER_H_
#define TERMINAL_UI_DENSITY_PRINTER_H_

#include <memory>
#include <unordered_map>
#include <ostream>
#include <vector>
#include <functional>

#include "Viewport.h"

class Grid;
class IDensityBuilder;

class DensityPrinter {
public:
    using Formatter = std::function<void 
            (double, double, double, 
             const std::vector<double>&, const DensityPrinter& printer)>;

    DensityPrinter(int cells_wide, int cells_high, const Viewport& viewport);
    DensityPrinter(int cells_wide, int cells_high, const Viewport& viewport,
        Formatter formatter, std::unique_ptr<IDensityBuilder> density_builder);

    ~DensityPrinter() = default;

    DensityPrinter(const DensityPrinter& other) = delete;
    DensityPrinter(DensityPrinter&& other) = default;
    DensityPrinter& operator =(const DensityPrinter& other) = delete;
    DensityPrinter& operator =(DensityPrinter&& other) = default;

    void draw_state(std::ostream& stream, const Grid& grid) const;

    const Viewport& viewport() const {return m_viewport;}
    double cell_width() const {return m_cell_width;}
    double cell_height() const {return m_cell_height;}
    int cells_wide() const {return m_cells_wide;}
    int cells_high() const {return m_cells_high;}

private:
    void print_density(std::ostream& stream, const std::vector<double>& density_grid,
            double total_density, const Grid& grid) const;

    Viewport m_viewport;
    int m_cells_wide;
    int m_cells_high;
    double m_cell_width;
    double m_cell_height;

    Formatter m_formatter;
    std::unique_ptr<IDensityBuilder> m_density_builder;
};

class IDensityBuilder {
public:
    IDensityBuilder() = default;
    virtual ~IDensityBuilder() = default;

    virtual void build_density_grid(std::vector<double>& density_grid, double& total_density,
            const Grid& grid, const DensityPrinter& printer) = 0;
};

class ParticleDensityBuilder: public IDensityBuilder {
public:
    ParticleDensityBuilder() = default;
    virtual ~ParticleDensityBuilder() = default;

    virtual void build_density_grid(std::vector<double>& density_grid, double& total_density,
        const Grid& grid, const DensityPrinter& printer) override;
};

class DefaultDensityFormatter {
public:
    void operator ()(double max_val, double cell_val, double total_sum, 
            const std::vector<double>& grid, const DensityPrinter& printer);
};

#endif
