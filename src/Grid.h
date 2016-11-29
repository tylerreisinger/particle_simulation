#ifndef PS_GRID_H_
#define PS_GRID_H_

#include <cassert>
#include <vector>
#include <unordered_map>
#include <memory>

#include "CommonTypes.h"
#include "Vector2.h"
#include "Particle.h"

class GridParticle;

class GridCell {
public:
    using iterator = std::unordered_map<int, GridParticle*>::iterator;
    using const_iterator = std::unordered_map<int, GridParticle*>::const_iterator;

    GridCell(int x, int y, int idx);
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

    int grid_index() const {return m_idx;}

private:
    std::unordered_map<int, GridParticle*> m_particles;    

    int m_cell_x;
    int m_cell_y;
    int m_idx;
};

class GridParticle {
    friend class GridCell;
public:
    GridParticle(const Particle& particle);
    GridParticle(Particle&& particle);
    ~GridParticle() = default;

    GridParticle(const GridParticle& other) = default;
    GridParticle(GridParticle&& other) noexcept = default;
    GridParticle& operator =(const GridParticle& other) = default;
    GridParticle& operator =(GridParticle&& other) noexcept = default;

    int id() const {
        return m_particle.id();
    }

    SpatialVector position() const {
        return m_particle.position();
    }

    SpatialVector velocity() const {
        return m_particle.velocity();
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
    using ParticleContainer = std::unordered_map<int, std::unique_ptr<GridParticle>>;
    using iterator = ParticleContainer::iterator;
    using const_iterator = ParticleContainer::const_iterator;

    Grid(PositionType width, PositionType height, PositionType xres, PositionType yres);

    ~Grid() = default;

    Grid(const Grid& other) = delete;
    Grid(Grid&& other) noexcept = default;
    Grid& operator =(const Grid& other) = delete;
    Grid& operator =(Grid&& other) noexcept = default;

    iterator begin() {return m_particles.begin();}
    iterator end() {return m_particles.end();}
    const_iterator begin() const {return m_particles.begin();}
    const_iterator end() const {return m_particles.end();}

    PositionType width() const {return m_width;}
    PositionType height() const {return m_height;}
    PositionType xres() const {return m_xres;}
    PositionType yres() const {return m_yres;}
    PositionType dx() const {return m_dx;}
    PositionType dy() const {return m_dy;}

    std::size_t num_particles() const {return m_particles.size();}

    std::size_t num_cells() const {return m_cells.size();}

    void add(const Particle& particle);
    void add(Particle&& particle);

    constexpr std::size_t position_to_cell(const SpatialVector& pos) const {
        auto x = static_cast<int>(pos.x / m_dx); 
        auto y = static_cast<int>(pos.y / m_dy);

        assert(x < m_xres);
        assert(y < m_yres);
        assert(x >= 0);
        assert(y >= 0);

        return x + y*m_xres;
    }

    bool is_point_within(const SpatialVector& pos) const {
        return (pos.x >= 0 && pos.y >= 0 && pos.x < m_width && pos.y < m_height);
    }

    GridCell& cell(std::size_t x, std::size_t y);
    GridCell& cell(std::size_t idx);

    void next_frame();
    void update_particle(GridParticle& particle);

    std::ostream& print_particle_density(std::ostream& stream, int level=0) const;

private:
    iterator insert(const Particle& m_particle);
    iterator insert(Particle&& particle);
    iterator insert(std::unique_ptr<GridParticle> m_particle);
    void remove_from_grid(GridParticle& m_particle);
    void update_forces();

    void apply_insert_list();
    void apply_delete_list();

    std::vector<GridCell> m_cells;

    std::unordered_map<int, std::unique_ptr<GridParticle>> m_particles;

    std::vector<GridParticle*> m_deleteList;
    std::vector<std::unique_ptr<GridParticle>> m_insertList;

    void build_grid();

    PositionType m_width;
    PositionType m_height;
    PositionType m_xres;
    PositionType m_yres;
    PositionType m_dx;
    PositionType m_dy;
};

#endif
