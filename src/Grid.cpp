#include "Grid.h"

#include <iostream>
#include <cassert>

GridParticle::GridParticle(const Particle& particle):
    m_particle(particle)
{}

GridParticle::GridParticle(Particle&& particle):
    m_particle(std::move(particle)) 
{} 
 
Grid::Grid(PositionType width, PositionType height, PositionType xres, PositionType yres):
    m_width(width), m_height(height), m_xres(xres), m_yres(yres), 
    m_dx(width / xres), m_dy(height / yres)
{
    build_grid(); 
}
 
Grid::iterator Grid::insert(const Particle& particle) {
    return insert(std::make_unique<GridParticle>(particle));
}

Grid::iterator Grid::insert(Particle&& particle) {
    return insert(std::make_unique<GridParticle>(std::move(particle)));
}

Grid::iterator Grid::insert(std::unique_ptr<GridParticle> p) {
    auto idx = position_to_cell(p->particle().position());  
    auto& cell = m_cells[idx];
    auto id = p->particle().id();
    auto it = m_particles.insert(std::make_pair(id, std::move(p)));
    assert(it.second);
    auto& item = it.first;
    cell.insert(item->second.get());
    return it.first;
}
 
void Grid::add(const Particle& particle) {
    m_insertList.emplace_back(std::make_unique<GridParticle>(particle)); 
}
 
void Grid::add(Particle&& particle) {
    m_insertList.emplace_back(std::make_unique<GridParticle>(std::move(particle))); 
}
 
GridCell& Grid::cell(std::size_t x, std::size_t y) { 
    assert(x+y*m_xres < m_cells.size());
    return m_cells[x + y*m_xres];
}
 
GridCell& Grid::cell(std::size_t idx) {
    assert(idx < m_cells.size());
    return m_cells[idx]; 
}
 
void Grid::next_frame() {
    apply_insert_list();
    apply_delete_list();
    for(auto& item : m_particles) {
        auto& p = item.second;
        p->particle().apply_update();
    }
}
 
void Grid::update_particle(GridParticle& particle) {
    auto cell_idx = position_to_cell(particle.particle().next_position());
    auto cell = particle.containing_cell();
    if(cell_idx != cell->grid_index()) {
        /*std::cout << 
            cell->grid_index() << " -> " << cell_idx << std::endl;*/
        cell->remove(particle); 
        this->cell(cell_idx).insert(&particle);
    }
}
 
std::ostream& Grid::print_particle_density(std::ostream& stream, int level) const {
    for(int y = 0; y < m_yres; ++y) {
        for(int x = 0; x < m_xres; ++x) {
            stream << m_cells[x + y*m_xres].size() << " ";
        }
        stream << "\n";
    } 
    return stream;
}
 
void Grid::remove_from_grid(GridParticle& item) {
    item.containing_cell()->remove(item);
    m_particles.erase(item.id()); 
}
 
void Grid::apply_insert_list() {
    for(auto& particle : m_insertList) {
        insert(std::move(particle));
    } 
    m_insertList.clear();
}
 
void Grid::apply_delete_list() {
    for(auto& particle : m_deleteList) {
        remove_from_grid(*particle);
    } 
    m_deleteList.clear();
}
 
void Grid::build_grid() {
    m_cells.reserve(m_xres*m_yres); 
    for(int y = 0; y < m_yres; ++y) {
        for(int x = 0; x < m_xres; ++x) {
            m_cells.emplace_back(x, y, x + y*m_xres);
        }
    }
}
 
GridCell::iterator GridCell::insert(GridParticle* particle) {
    particle->m_owning_cell = this;
    return m_particles.push_back(particle);
}

GridCell::GridCell(int x, int y, int idx):
    m_cell_x(x), m_cell_y(y), m_idx(idx) {
 
}
 
void GridCell::remove(GridParticle& particle) {
    m_particles.erase(particle);
}
 
 
