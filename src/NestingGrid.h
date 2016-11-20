#ifndef PS_NESTING_GRID_H_
#define PS_NESTING_GRID_H_

#include <cassert>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "CommonTypes.h"
#include "Vector2.h"

template<typename Elem>
class WorldUpdater;

template<typename Elem>
class GridCell {
public:
    GridCell() {};
    ~GridCell() = default;

    GridCell(const GridCell& other) = delete;
    GridCell(GridCell&& other) noexcept = default;
    GridCell& operator =(const GridCell& other) = delete;
    GridCell& operator =(GridCell&& other) noexcept = default;

    int particle_num() const {
        return m_particle_num;
    }

private:
    SpatialVector m_force_center = {0, 0};
    ForceType m_force_strength = 0;
    int m_particle_num = 0;
};

template<typename Elem>
class PopulatedGridCell {
public:
    using iterator = typename std::vector<Elem*>::iterator;
    using const_iterator = typename std::vector<Elem*>::const_iterator;    

    PopulatedGridCell() {};
    ~PopulatedGridCell() = default;

    PopulatedGridCell(const PopulatedGridCell& other) = delete;
    PopulatedGridCell(PopulatedGridCell&& other) noexcept = default;
    PopulatedGridCell& operator =(const PopulatedGridCell& other) = delete;
    PopulatedGridCell& operator =(PopulatedGridCell&& other) noexcept = default;

    iterator insert(Elem* elem) {
        m_elements.push_back(elem);
        return m_elements.end()-1;
    }

    std::size_t num_elems() const {
        return m_elements.size();
    }

private:
    std::vector<Elem*> m_elements;
};

template<typename Elem>
class NestingGridLayer {
public:
    using iterator = typename std::vector<GridCell<Elem>>::iterator;
    using const_iterator = typename std::vector<GridCell<Elem>>::const_iterator;

    NestingGridLayer(PositionType width, PositionType height, PositionType xres,
            PositionType yres);
    ~NestingGridLayer() = default;

    NestingGridLayer(const NestingGridLayer& other) = delete;
    NestingGridLayer(NestingGridLayer&& other) noexcept = default;
    NestingGridLayer& operator =(const NestingGridLayer& other) = delete;
    NestingGridLayer& operator =(NestingGridLayer&& other) noexcept = default;

    GridCell<Elem>& operator [](std::size_t idx) {
        return m_cells[idx];
    }
    const GridCell<Elem>& operator [](std::size_t idx) const {
        return m_cells[idx];
    }

    iterator begin() {
        return m_cells.begin();
    }
    const_iterator begin() const {
        return m_cells.begin();
    }
    const_iterator cbegin() const {
        return m_cells.cbegin();
    }

    iterator end() {
        return m_cells.end();
    }
    const_iterator end() const {
        return m_cells.end();
    }
    const_iterator cend() const {
        return m_cells.cend();
    }

    PositionType xres() const {
        return m_xres;
    }
    PositionType yres() const {
        return m_xres;
    }
    PositionType dx() const {
        return m_dx;
    }
    PositionType dy() const {
        return m_dy;
    }

    constexpr std::size_t num_cells() const {
        return m_xres*m_yres;
    }

private:
    std::vector<GridCell<Elem>> m_cells;

    PositionType m_width;
    PositionType m_height;
    PositionType m_xres;
    PositionType m_yres; 

    PositionType m_dx;
    PositionType m_dy;
};

template<typename Elem>
class NestingGrid {

template<typename U>
friend class WorldUpdater;

public:
    using iterator = typename std::vector<GridCell<Elem>>::iterator;
    using const_iterator = typename std::vector<GridCell<Elem>>::const_iterator;

    NestingGrid(PositionType width, PositionType height, PositionType xres,
            PositionType yres, int num_layers);
    ~NestingGrid() = default;

    NestingGrid(const NestingGrid& other) = delete;
    NestingGrid(NestingGrid&& other) noexcept = delete;
    NestingGrid& operator =(const NestingGrid& other) = delete;
    NestingGrid& operator =(NestingGrid&& other) noexcept = delete;

    PopulatedGridCell<Elem>& add_element(Elem element) {
        auto idx = position_to_cell(element.position());    
        auto& cell = m_cells[idx];
        auto id = element.id();
        auto& particle = *m_elements.
            insert(std::make_pair(id, std::move(element))).first;
        cell.insert(&particle.second);
        return cell;
    }

    PositionType xres() const {
        return m_xres;
    }
    PositionType yres() const {
        return m_xres;
    }
    PositionType dx() const {
        return m_dx;
    }
    PositionType dy() const {
        return m_dy;
    }

    constexpr std::size_t position_to_cell(const SpatialVector& pos) const {
        auto x = pos.x / m_dx; 
        auto y = pos.y / m_dy;

        assert(x < m_xres);
        assert(y < m_yres);
        assert(x >= 0);
        assert(y >= 0);

        return x + y*m_width;
    }

    std::ostream& print_particle_density(std::ostream& stream, int layer_num) {
        const auto& layer = m_layers[layer_num];
        auto xres = layer.xres();
        auto yres = layer.yres();

        for(int y = 0; y < yres; ++y) {
            for(int x = 0; x < xres; ++x) {
                stream << layer[x + y*xres].particle_num() << " ";
            }
            stream << "\n";
        }
        return stream;
    }

    void begin_update() {

    }

private:
    void build_layers(int num_layers);

    void update_complete(WorldUpdater<Elem>& updater) {
        for(auto& particle: updater.particles()) {
              
        }
    }

    std::vector<NestingGridLayer<Elem>> m_layers;
    std::vector<PopulatedGridCell<Elem>> m_cells;
    std::unordered_map<int, Elem> m_elements;

    PositionType m_width;
    PositionType m_height;
    PositionType m_xres;
    PositionType m_yres; 

    PositionType m_dx;
    PositionType m_dy;
};

template<typename Elem>
class WorldUpdater {
public:
    WorldUpdater(std::vector<PopulatedGridCell<Elem>> values, 
            NestingGrid<Elem>& grid):
        m_grid(&grid) 
    {}

    ~WorldUpdater() {
        finalize();
    }

    WorldUpdater(const WorldUpdater& other) = delete;
    WorldUpdater(WorldUpdater&& other) noexcept {
        if(m_grid != nullptr) {
            finalize();
        }
        std::swap(m_grid, other.m_grid);
        m_cells = std::move(other.m_cells);
    }
    WorldUpdater& operator =(const WorldUpdater& other) = delete;
    WorldUpdater& operator =(WorldUpdater&& other) noexcept {
        if(m_grid != nullptr) {
            finalize();
        }
        std::swap(m_grid, other.m_grid);
        m_cells = std::move(other.m_cells);
        return *this;
    }

    std::vector<PopulatedGridCell<Elem>>& cells() {
        return m_cells;
    }

    PopulatedGridCell<Elem>& operator [](std::size_t idx) {
        assert(m_grid != nullptr);
        return m_cells[idx];
    }
    const PopulatedGridCell<Elem>& operator [](std::size_t idx) const {
        assert(m_grid != nullptr);
        return m_cells[idx];
    }

    void update_complete() {
        finalize();
    }

private:
    void finalize() {
        if(m_grid != nullptr) {
            m_grid->update_complete(*this); 
            m_grid = nullptr;
        }
    }

    std::vector<PopulatedGridCell<Elem>> m_cells;
    NestingGrid<Elem>* m_grid = nullptr;
};

template<typename Elem>
inline NestingGrid<Elem>::NestingGrid(PositionType width, PositionType height,
        PositionType xres, PositionType yres, int num_layers):
    m_width(width), m_height(height), m_xres(xres), m_yres(yres),
    m_dx(width / xres), m_dy(height / yres)
{
    build_layers(num_layers);
}
 
template<typename Elem>
inline void NestingGrid<Elem>::build_layers(int num_layers)
{
    PositionType xres = m_xres;
    PositionType yres = m_yres;
    for(int i = 0; i < num_layers; ++i) {
        xres /= 2.0;
        yres /= 2.0;

        m_layers.emplace_back(m_width, m_height, xres, yres);
    }
    m_cells.resize(m_xres*m_yres);
}
 
template<typename Elem>
inline NestingGridLayer<Elem>::NestingGridLayer(PositionType width, PositionType height, 
        PositionType xres, PositionType yres):
    m_width(width), m_height(height), m_xres(xres), m_yres(yres),
    m_dx(width / xres), m_dy(height / yres)
{
    m_cells.resize(m_xres*m_yres);
}

template<typename Elem>
typename NestingGridLayer<Elem>::iterator begin(NestingGridLayer<Elem>& target) {
    return target.begin();
}
template<typename Elem>
typename NestingGridLayer<Elem>::const_iterator begin(const NestingGridLayer<Elem>& target) {
    return target.begin();
}

template<typename Elem>
typename NestingGridLayer<Elem>::iterator end(NestingGridLayer<Elem>& target) {
    return target.end();
}
template<typename Elem>
typename NestingGridLayer<Elem>::const_iterator end(const NestingGridLayer<Elem>& target) {
    return target.end();
}
 
#endif
