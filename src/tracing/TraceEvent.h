#ifndef PS_PARTICLETRACEEVENT_H_
#define PS_PARTICLETRACEEVENT_H_

#include <ostream>
#include <chrono>

#include "../SimulationTime.h"
#include "../Vector2.h"
#include "../CommonTypes.h"

class Simulation;
class Particle;

namespace tracing {

enum class TraceEventType {
    FrameBegin,
    FrameEnd,
    ParticleFrameBegin,
    ParticleFrameEnd,
    WallCollideBegin,
    WallCollideEnd,
    WallBounce,
    SimulateRemainder,
    MotionParamsUpdated,
};


class TraceEvent;
class ParticleTraceEvent;
class ParticleMotionTraceEvent;

class TraceEventVisitor {
public:
    TraceEventVisitor() = default;
    virtual ~TraceEventVisitor() {};

    virtual void visit(TraceEvent& evt) {};
    virtual void visit(ParticleTraceEvent& evt) {};
    virtual void visit(ParticleMotionTraceEvent& evt) {};
};

class TraceEvent {
public:
    using Clock = std::chrono::system_clock;
    using TimeStamp = Clock::duration;

    TraceEvent(TraceEventType type, const Simulation* simulation,
            SimulationTime time);

    virtual ~TraceEvent() {};

    TraceEvent(const TraceEvent& other) = delete;
    TraceEvent(TraceEvent&& other) noexcept = default;
    TraceEvent& operator =(const TraceEvent& other) = delete;
    TraceEvent& operator =(TraceEvent&& other) noexcept = default;

    TraceEventType type() const {return m_type;}
    const Simulation* simulation() const {return m_simulation;}
    const SimulationTime& simulation_time() const {return m_sim_time;}

    virtual void accept(TraceEventVisitor& visitor) {
        visitor.visit(*this);
    }

private:
    TraceEventType m_type;
    const Simulation* m_simulation;
    SimulationTime m_sim_time;
};

class ParticleTraceEvent: public TraceEvent {
public:
    ParticleTraceEvent(TraceEventType type, const Simulation* simulation,
            SimulationTime time, int particle_id, const Particle* particle);

    virtual ~ParticleTraceEvent() {};

    ParticleTraceEvent(ParticleTraceEvent&& other) noexcept = default;
    ParticleTraceEvent& operator =(ParticleTraceEvent&& other) noexcept = default;

    virtual void accept(TraceEventVisitor& visitor) override {
        visitor.visit(*this);
    }

    const Particle* particle() const {return m_particle;}
    int particle_id() const {return m_particle_id;}
    
private:
    int m_particle_id;
    const Particle* m_particle;
};

class ParticleMotionTraceEvent: public ParticleTraceEvent {
public:
    ParticleMotionTraceEvent(TraceEventType type, const Simulation* simulation,
            SimulationTime time, int particle_id, const Particle* particle,
            double m_dt, SpatialVector acceleration, SpatialVector start_pos, 
            SpatialVector start_vel, SpatialVector end_pos,
            SpatialVector end_vel);
    ParticleMotionTraceEvent(TraceEventType type, const Simulation* simulation,
            SimulationTime time, int particle_id, const Particle* particle,
            double m_dt, SpatialVector acceleration, SpatialVector start_pos, 
            SpatialVector start_vel);
    ParticleMotionTraceEvent(TraceEventType type, const Simulation* simulation,
            SimulationTime time, int particle_id, const Particle* particle,
            double m_dt, SpatialVector acceleration);
    
    virtual ~ParticleMotionTraceEvent() {};

    ParticleMotionTraceEvent(ParticleMotionTraceEvent&& other) noexcept = default;
    ParticleMotionTraceEvent& operator =(ParticleMotionTraceEvent&& other) noexcept = default;

    virtual void accept(TraceEventVisitor& visitor) override {
        visitor.visit(*this);
    }

    const SpatialVector& acceleration() const {return m_acceleration;}
    const SpatialVector& starting_position() const {return m_start_pos;}
    const SpatialVector& ending_position() const {return m_end_pos;}
    const SpatialVector& starting_velocity() const {return m_start_vel;}
    const SpatialVector& ending_velocity() const {return m_end_vel;}

    double update_dt() const {return m_dt;}

    bool has_moved() const {return m_start_pos == m_end_pos;}

    void set_starting_motion_params(const SpatialVector& position,
            const SpatialVector& velocity) {
        m_start_pos = position;
        m_start_vel = velocity;
    }

    void set_ending_motion_params(const SpatialVector& position,
            const SpatialVector& velocity) {
        m_end_pos = position;
        m_end_vel = velocity;
    }

private:
    double m_dt;
    SpatialVector m_acceleration;
    SpatialVector m_start_pos;
    SpatialVector m_start_vel;
    SpatialVector m_end_pos;
    SpatialVector m_end_vel;
};

std::ostream& operator <<(std::ostream& stream, TraceEventType type);

}

#endif
