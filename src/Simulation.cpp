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
}
 
void Simulation::on_particle_out_of_boundry(Particle& particle,
        SpatialVector& acceleration) {

    std::cout << "Particle " << particle.id() << " out of bounds.\n";
    resolve_border_collision(particle,acceleration);
}

void Simulation::resolve_border_collision(Particle& particle, 
        SpatialVector& acceleration) {
    double remaining_time = m_simulation_time.time_delta();

    resolve_border_collision_recursive(particle, remaining_time, acceleration);

    if(remaining_time > 0) {
        euler(particle, remaining_time, acceleration);
        std::cout << "Finishing remaining simulation. \n";
    }
}

void Simulation::resolve_border_collision_recursive(Particle& particle, 
        double& remaining_time, SpatialVector& acceleration) {

    auto velocity = particle.next_velocity();
    auto pos = particle.next_position();

    double x_time;
    double y_time;

    if(velocity.x < 0) {
        x_time = -pos.x / velocity.x;
    } else {
        x_time = (m_grid.width() - pos.x) / velocity.x;
    }

    if(velocity.y < 0) {
        y_time = -pos.y / velocity.y;
    } else {
        y_time = (m_grid.height() - pos.y) / velocity.y;
    }

    if(x_time >= 0 && x_time < y_time) {
        if(x_time < remaining_time) {
            euler(particle, x_time, acceleration);

            particle.next_velocity().x = -particle.next_velocity().x;

            remaining_time -= x_time;
            resolve_border_collision_recursive(particle, remaining_time,
                acceleration);
        }
    } else if(y_time >= 0) {
        if(y_time < remaining_time) {
            euler(particle, y_time, acceleration);

            particle.next_velocity().y = -particle.next_velocity().y;

            remaining_time -= y_time;
            resolve_border_collision_recursive(particle, remaining_time,
                acceleration);
        }
    }
}
 
void Simulation::advance_physics(Particle& particle, double dt, SpatialVector acceleration) {
    euler(particle, dt, acceleration);
    if(!m_grid.is_point_within(particle.next_position())) {
        //If we detect a boundary collision, we undo the frame simulation and
        //run a more detailed simulation that breaks the movement up into segments
        //between each collision.
        particle.reset_position();
        particle.reset_velocity();
        on_particle_out_of_boundry(particle, acceleration);
    }
}
 
void Simulation::euler(Particle& particle, double dt, SpatialVector acceleration) {
    auto vel = particle.next_velocity() + acceleration*dt;
    auto pos = particle.next_position() + particle.next_velocity()*dt; 

    particle.update_velocity(vel);
    particle.update_position(pos);
}
 
