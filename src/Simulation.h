#ifndef PS_SIMULATION_H_
#define PS_SIMULATION_H_

#include "Grid.h"
#include "SimulationTime.h"

#include "tracing/Tracer.h"

class Simulation {
public:
    using SpatialContainer = Grid;

    Simulation(SpatialContainer&& grid,
            double base_time_step = 1.0);
    ~Simulation() = default;

    Simulation(const Simulation& other) = delete;
    Simulation(Simulation&& other) noexcept = delete;
    Simulation& operator =(const Simulation& other) = delete;
    Simulation& operator =(Simulation&& other) noexcept = delete;

    void do_frame();

    double base_time_step() const {return m_base_time_step;}
    void set_base_time_step(double value) {m_base_time_step = value;}

    SpatialContainer& get_particles() {
        return m_grid;
    }

    const SpatialContainer& get_particles() const {
        return m_grid;
    }
   
private: 
#ifdef TRACING
    void setup_tracing();
#endif

    void on_particle_out_of_boundry(Particle& particle, SpatialVector& acceleration);
    void resolve_border_collision(Particle& particle, SpatialVector& acceleration);
    void resolve_border_collision_recursive(Particle& particle, 
            double& remaining_time, SpatialVector& acceleration);

    ForceType compute_exact_force(const Particle& particle);
    ForceType compute_acceleration_from_force(const Particle& particle, 
            const ForceType& force) const;

    void advance_physics(Particle& particle, double dt, SpatialVector acceleration);
    void euler(Particle& particle, double dt, SpatialVector acceleration);
    void euler(const Particle& particle, double dt, SpatialVector acceleration,
            SpatialVector& position, SpatialVector& velocity);

    SpatialContainer m_grid;        
    SimulationTime m_simulation_time;
    double m_base_time_step = 1.0;

#ifdef TRACING
    tracing::Tracer m_tracer;
#endif
};

#endif
