#ifndef TERMINAL_UI_DENSITY_PRINTER_H_
#define TERMINAL_UI_DENSITY_PRINTER_H_

#include <memory>
#include <ostream>
#include <vector>
#include <functional>

#include "Viewport.h"
#include "BasicGridPrinter.h"

class Grid;

namespace tui {

class IDensityBuilder;

class DensityPrinter: public BasicGridPrinter {
public:
    using Formatter = std::function<void 
            (double, double, double, 
             const std::vector<double>&, const DensityPrinter& printer)>;

    DensityPrinter(int width, int height, const Viewport& viewport);
    DensityPrinter(int width, int height, const Viewport& viewport,
        Formatter formatter, std::unique_ptr<IDensityBuilder> density_builder);

    ~DensityPrinter() = default;

    DensityPrinter(const DensityPrinter& other) = delete;
    DensityPrinter(DensityPrinter&& other) = default;
    DensityPrinter& operator =(const DensityPrinter& other) = delete;
    DensityPrinter& operator =(DensityPrinter&& other) = default;

    virtual void print_grid(std::ostream& stream, const Grid& grid) const override;

private:
    void print_density(std::ostream& stream, const std::vector<double>& density_grid,
            double total_density, const Grid& grid) const;

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

}

#endif
