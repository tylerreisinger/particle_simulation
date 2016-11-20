#ifndef PS_GRID_H_
#define PS_GRID_H_

#include <cassert>
#include <vector>
#include <unordered_map>

#include "CommonTypes.h"
#include "Vector2.h"
#include "Particle.h"

class GridParticle;

class GridCell {
public:
    using iterator = std::unordered_map<int, GridParticle*>::iterator;
    using const_iterator = std::unordered_map<int, GridParticle*>::const_iterator;

    GridCell() = default;
    ~GridCell() = default;

    GridCell(const GridCell& other) = delete;
    GridCell(GridCell&& other) noexcept = default;
    GridCell& operator =(const GridCell& other) = delete;
    GridCell& operator =(GridCell&& other) noexcept = default;

    iterator begin() {return m_particles.begin();}
    iterator end() {return m_particles.end();}
    const_iterator begin() const {return m_particles.begin();}
    const_iterator end() const {return m_particles.end();}

    iterator insert(GridParticle* particle);

    void remove(GridParticle& particle);

    std::size_t size() const {
        return m_particles.size();
    }

private:
    std::unordered_map<int, GridParticle*> m_particles;    
};

class GridParticle {
    friend class Grid;
public:
    GridParticle(const Particle& particle);
    ~GridParticle() = default;

    GridParticle(const GridParticle& other) = default;
    GridParticle(GridParticle&& other) noexcept = default;
    GridParticle& operator =(const GridParticle& other) = default;
    GridParticle& operator =(GridParticle&& other) noexcept = default;

    int id() const {
        return m_particle.id();
    }

    Particle& particle() {
        return m_particle;
    }
    
    const Particle& particle() const {
        return m_particle;
    }

    GridCell* containing_cell() const {
        return m_owning_cell;
    }

private:
    Particle m_particle;
    GridCell* m_owning_cell = nullptr;
};

class Grid {
public:
    using iterator = std::unordered_map<int, GridParticle>::iterator;
    using const_iterator = std::unordered_map<int, GridParticle>::iterator;

    Grid(PositionType width, PositionType height, PositionType xres, PositionType yres);

    ~Grid() = default;

    Grid(const Grid& other) = delete;
    Grid(Grid&& other) noexcept = default;
    Grid& operator =(const Grid& other) = delete;
    Grid& operator =(Grid&& other) noexcept = default;

    PositionType width() const {return m_width;}
    PositionType height() const {return m_height;}
    PositionType xres() const {return m_xres;}
    PositionType yres() const {return m_yres;}
    PositionType dx() const {return m_dx;}
    PositionType dy() const {return m_dy;}

    void add(const Particle& particle);

    constexpr std::size_t position_to_cell(const SpatialVector& pos) const {
        auto x = static_cast<int>(pos.x / m_dx); 
        auto y = static_cast<int>(pos.y / m_dy);

        assert(x < m_xres);
        assert(y < m_yres);
        assert(x >= 0);
        assert(y >= 0);

        return x + y*m_width;
    }

    GridCell& cell(std::size_t x, std::size_t y);
    GridCell& cell(std::size_t idx);

    void next_frame();

    std::ostream& print_particle_density(std::ostream& stream, int level=0) const;

private:
    iterator insert(const Particle& m_particle);
    iterator insert(GridParticle&& m_particle);
    void remove_from_grid(GridParticle& m_particle);

    void apply_insert_list();
    void apply_delete_list();

    std::vector<GridCell> m_cells;

    std::unordered_map<int, GridParticle> m_particles;

    std::vector<GridParticle*> m_deleteList;
    std::vector<GridParticle> m_insertList;

    void build_grid();

    PositionType m_width;
    PositionType m_height;
    PositionType m_xres;
    PositionType m_yres;
    PositionType m_dx;
    PositionType m_dy;
};

#endif
