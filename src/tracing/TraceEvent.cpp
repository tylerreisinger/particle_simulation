#include "TraceEvent.h"


namespace tracing {
 
TraceEvent::TraceEvent(TraceEventType type, const Simulation* simulation,
        SimulationTime time):
    m_type(type), m_simulation(simulation), m_sim_time(time)
{}
 
ParticleTraceEvent::ParticleTraceEvent(TraceEventType type, 
        const Simulation* simulation, SimulationTime time,
        int particle_id, const Particle* particle):
    TraceEvent(type, simulation, time), 
    m_particle_id(particle_id), m_particle(particle)
{}

std::string to_string(tracing::TraceEventType type) {
    switch(type) {
    case TraceEventType::FrameBegin: 
        return "FrameBegin";
    case TraceEventType::FrameEnd:
        return "FrameEnd";
    case TraceEventType::ParticleFrameBegin:
        return "ParticleFrameBegin";
    case TraceEventType::ParticleFrameEnd:
        return "ParticleFrameEnd";
    case TraceEventType::WallCollideBegin:
        return "WallCollideBegin";
    case TraceEventType::WallCollideEnd:
        return "WallCollideEnd";
    case TraceEventType::WallBounce:
        return "WallBounce";
    case TraceEventType::CollisionEnergyLoss:
        return "CollisionEnergyLoss";
    case TraceEventType::SimulateRemainder:
        return "SimulateRemainder";
    case TraceEventType::MotionParamsUpdated:
        return "MotionParamsUpdated";
    default:
        return "Unknown Event";
    }
}

std::ostream& operator <<(std::ostream& stream, TraceEventType type) {
    stream << to_string(type);
    return stream;
}
 
 
ParticleMotionTraceEvent::ParticleMotionTraceEvent(
        TraceEventType type, const Simulation* simulation, 
        SimulationTime time, int particle_id, const Particle* particle, 
        double dt, SpatialVector acceleration, SpatialVector start_pos, 
        SpatialVector start_vel, SpatialVector end_pos, 
        SpatialVector end_vel):
    ParticleTraceEvent(type, simulation, time, particle_id, particle), 
    m_dt(dt), m_acceleration(acceleration), m_start_pos(start_pos), m_start_vel(start_vel), 
    m_end_pos(end_pos), m_end_vel(end_vel) {
 
}
 
ParticleMotionTraceEvent::ParticleMotionTraceEvent(TraceEventType type, 
        const Simulation* simulation, SimulationTime time, int particle_id, 
        const Particle* particle, double dt, SpatialVector acceleration, 
        SpatialVector start_pos, SpatialVector start_vel):
    ParticleTraceEvent(type, simulation, time, particle_id, particle), 
    m_dt(dt), m_acceleration(acceleration), m_start_pos(start_pos), m_start_vel(start_vel)
{
 
}
 
ParticleMotionTraceEvent::ParticleMotionTraceEvent(TraceEventType type, 
        const Simulation* simulation, SimulationTime time, int particle_id, 
        const Particle* particle, double dt, SpatialVector acceleration):
    ParticleTraceEvent(type, simulation, time, particle_id, particle),
    m_dt(dt), m_acceleration(acceleration) {
 
}
 
}
