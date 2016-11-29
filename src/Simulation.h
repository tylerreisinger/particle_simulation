#ifndef PS_SIMULATION_H_
#define PS_SIMULATION_H_

#include "Grid.h"
#include "SimulationTime.h"

class Simulation {
public:
    using SpatialContainer = Grid;

    explicit Simulation(SpatialContainer&& grid);
    ~Simulation() = default;

    Simulation(const Simulation& other) = delete;
    Simulation(Simulation&& other) noexcept = delete;
    Simulation& operator =(const Simulation& other) = delete;
    Simulation& operator =(Simulation&& other) noexcept = delete;

    void do_frame();

    SpatialContainer& get_particles() {
        return m_grid;
    }

    const SpatialContainer& get_particles() const {
        return m_grid;
    }
   
private: 
    void on_particle_out_of_boundry(Particle& particle);

    void advance_physics(Particle& particle, double dt, SpatialVector acceleration);
    void euler(Particle& particle, double dt, SpatialVector acceleration);
    //void runge_kutta4(Particle& particle, double dt, SpatialVector acceleration);

    SpatialContainer m_grid;        
    SimulationTime m_simulation_time;
};

#endif
