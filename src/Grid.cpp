#include "Grid.h"


GridParticle::GridParticle(const Particle& particle):
    m_particle(particle)
{}
 
Grid::Grid(PositionType width, PositionType height, PositionType xres, PositionType yres):
    m_width(width), m_height(height), m_xres(xres), m_yres(yres), 
    m_dx(width / xres), m_dy(height / yres)
{
    build_grid(); 
}
 
Grid::iterator Grid::insert(const Particle& particle) {
    return insert(GridParticle(particle));
}

Grid::iterator Grid::insert(GridParticle&& p) {
    auto idx = position_to_cell(p.particle().position());  
    auto& cell = m_cells[idx];
    auto id = p.particle().id();
    p.m_owning_cell = &cell;
    auto it = m_particles.insert(std::make_pair(id, std::move(p))).first;
    auto& item = *it;
    cell.insert(&item.second);
    return it;
}
 
void Grid::add(const Particle& particle) {
    m_insertList.emplace_back(particle); 
}
 
GridCell& Grid::cell(std::size_t x, std::size_t y) { 
    return m_cells[x + y*m_xres];
}
 
GridCell& Grid::cell(std::size_t idx) {
    return m_cells[idx]; 
}
 
void Grid::next_frame() {
    apply_insert_list();
    apply_delete_list();
    for(auto& item : m_particles) {
        auto& p = item.second;
        p.particle().apply_update();
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
    m_cells.resize(m_xres*m_yres); 
}
 
GridCell::iterator GridCell::insert(GridParticle* particle) {
    return m_particles.insert(std::make_pair(particle->id(), particle)).first;
}

void GridCell::remove(GridParticle& particle) {
    m_particles.erase(particle.id());
}
 
