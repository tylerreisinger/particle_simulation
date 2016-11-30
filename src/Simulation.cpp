#include "Simulation.h"

#include <iostream>

Simulation::Simulation(SpatialContainer&& grid):
    m_grid(std::move(grid)) {
}
 
void Simulation::do_frame() {
    m_simulation_time.begin_frame(1.0);

    std::cout << "Frame start, t=" << m_simulation_time.current_simulation_time() << std::endl;


    for(int i = 0; i < m_grid.num_cells(); ++i) {
        auto& cell = m_grid.cell(i);
        for(auto& item : cell) {
            auto& particle = item;
            advance_physics(particle.particle(), m_simulation_time.time_delta(), 
                    SpatialVector::zero());
            /*std::cout << "Pos: " << particle.position() << "->" 
                << particle.particle().next_position() << "; Vel: " 
                << particle.velocity() << "->" << particle.particle().next_velocity()
                << " -- " << particle.containing_cell()->grid_index()
                << "\n";*/
        }
    }
    m_grid.next_frame();
    for(auto& particle : m_grid) {
        m_grid.update_particle(*particle.second);
    }
    for(int i = 0; i < m_grid.num_particles(); ++i) {
        
        /*auto& cell = m_grid.cell(i);
        for(auto& item : cell) {
            auto particle = item.second;
            m_grid.update_particle(*particle);
        }*/
    }

}
 
void Simulation::on_particle_out_of_boundry(Particle& particle) {
    auto new_position = particle.next_position();
    auto new_velocity = particle.next_velocity();
    /*std::cout << "OOB: " << particle.next_position() << "; " 
        << particle.next_velocity() << "... ";*/

    if(new_position.x < 0.0) {
        new_position.x = 0.0;
        new_velocity.x = -new_velocity.x;
    } else if(new_position.x >= m_grid.width()) {
        new_position.x = m_grid.width()-0.00001;
        new_velocity.x = -new_velocity.x;
    }
    if(new_position.y < 0.0) {
        new_position.y = 0.0;
        new_velocity.y = -new_velocity.y;
    } else if(new_position.y >= m_grid.height()) {
        new_position.y = m_grid.height()-0.00001;
        new_velocity.y = -new_velocity.y;
    }

    particle.update_position(new_position);
    particle.update_velocity(new_velocity);
    /*std::cout << "resolved to: " << particle.next_position() << "; " 
        << particle.next_velocity() << std::endl;*/
}
 
void Simulation::advance_physics(Particle& particle, double dt, SpatialVector acceleration) {
    euler(particle, dt, acceleration);
    if(!m_grid.is_point_within(particle.next_position())) {
        on_particle_out_of_boundry(particle);
    }
}
 
void Simulation::euler(Particle& particle, double dt, SpatialVector acceleration) {
    auto vel = particle.velocity() + acceleration*dt;
    auto pos = particle.position() + particle.velocity()*dt; 

    particle.update_velocity(vel);
    particle.update_position(pos);
}
 
