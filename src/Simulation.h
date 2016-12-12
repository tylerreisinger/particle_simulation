#ifndef PS_SIMULATION_H_
#define PS_SIMULATION_H_

#include <memory>

#include "Grid.h"
#include "SimulationTime.h"
#include "IBoundaryCollisionResolver.h"

#include "tracing/Tracer.h"

class IWorldPhysicsHandler;
class IMotionIntegrator;

class Simulation {
public:
    using SpatialContainer = Grid;

    Simulation(SpatialContainer&& grid,
            double base_time_step = 1.0);
    ~Simulation();

    Simulation(const Simulation& other) = delete;
    Simulation(Simulation&& other) noexcept = delete;
    Simulation& operator =(const Simulation& other) = delete;
    Simulation& operator =(Simulation&& other) noexcept = delete;

    Simulation& set_boundary_collision_resolver(
            std::unique_ptr<IBoundaryCollisionResolver> resolver);
    IBoundaryCollisionResolver& boundary_collision_resolver() {
        return *m_boundary_collision_resolver;
    }

    void do_frame();

    double base_time_step() const {return m_base_time_step;}
    void set_base_time_step(double value) {m_base_time_step = value;}

    const SimulationTime& simulation_time() const {return m_simulation_time;}

    SpatialContainer& get_particles() {
        return m_grid;
    }

    const SpatialContainer& get_particles() const {
        return m_grid;
    }

    void simulate_motion(Particle& particle, double dt, 
            const SpatialVector& acceleration);
    void simulate_motion(Particle& particle, double dt, SpatialVector acceleration,
            SpatialVector& position, SpatialVector& velocity);

    ForceType compute_acceleration(Particle& particle);
    ForceType compute_acceleration(Particle& particle, 
            const SpatialVector& updated_position, 
            const SpatialVector& updated_velocity);

#ifdef TRACING
    const tracing::Tracer& tracer() const {return m_tracer;}
#endif   
private: 
#ifdef TRACING
    void setup_tracing();
#endif
    std::unique_ptr<IBoundaryCollisionResolver> make_default_boundary_resolver();
    std::unique_ptr<IWorldPhysicsHandler> make_default_world_physics();
    std::unique_ptr<IMotionIntegrator> make_default_integrator();

    void on_particle_out_of_boundry(Particle& particle, SpatialVector& acceleration);

    ForceType compute_exact_force(const Particle& particle, 
            const SpatialVector& updated_position, const SpatialVector& updated_velocity);
    ForceType compute_acceleration_from_force(const Particle& particle, 
            const ForceType& force) const;

    void advance_physics(Particle& particle, double dt, SpatialVector acceleration);
    void advance_physics(Particle& particle, double dt, SpatialVector acceleration,
            SpatialVector& position, SpatialVector& velocity);

    std::unique_ptr<IBoundaryCollisionResolver> m_boundary_collision_resolver;
    std::unique_ptr<IWorldPhysicsHandler> m_world_physics;
    std::unique_ptr<IMotionIntegrator> m_integrator;

    SpatialContainer m_grid;        
    SimulationTime m_simulation_time;
    double m_base_time_step = 1.0;

#ifdef TRACING
    tracing::Tracer m_tracer;
#endif
};

#endif
