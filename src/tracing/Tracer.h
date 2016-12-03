#ifndef PS_TRACER_H_
#define PS_TRACER_H_

#ifdef TRACING

#define SIM_TRACER_EVENT(tracer, type, sim, time) \
    tracer.broadcast_event(tracing::TraceEvent(type, sim, time));

#define PARTICLE_TRACER_EVENT(tracer, type, particle, sim, time) \
    if(tracer.is_particle_active(particle)) { \
        tracer.broadcast_event(tracing::ParticleTraceEvent(type, sim, time, \
            particle.id(), &particle)); \
    } 

#define PARTICLE_MOTION_TRACER_EVENT(tracer, type, particle, sim, time, dt, \
        acceleration, update_fn) \
    if(tracer.is_particle_active(particle)) { \
        tracing::ParticleMotionTraceEvent evt(type, sim, time, \
                particle.id(), &particle, dt, acceleration, \
                particle.next_position(), particle.next_velocity()); \
        update_fn(); \
        evt.set_ending_motion_params(particle.next_position(), particle.next_velocity()); \
        tracer.broadcast_event(evt); \
    } else { \
        update_fn(); \
    }


#include <vector>
#include <memory>
#include <unordered_set>

#include "TraceEvent.h"
#include "TracerSink.h"
#include "Particle.h"

namespace tracing {

class Tracer {
public:
    Tracer();
    ~Tracer();

    Tracer(const Tracer& other) = delete;
    Tracer(Tracer&& other) noexcept = default;
    Tracer& operator =(const Tracer& other) = delete;
    Tracer& operator =(Tracer&& other) noexcept = default;

    void broadcast_event(TraceEvent& event);
    void broadcast_event(TraceEvent&& event);

    void attach_sink(std::unique_ptr<ITracerSink> sink);

    void remove_all_sinks();

    void enable_particle_tracing(const Particle& particle);
    void enable_particle_tracing(int id);
    void enable_particle_tracing(const std::vector<int>& ids);
    void enable_particle_tracing(std::initializer_list<int> ids);
    void disable_particle_tracing(const Particle& particle);
    void disable_particle_tracing(int id);
    void disable_particle_tracing(const std::vector<int>& ids);
    void disable_particle_tracing(std::initializer_list<int> ids);

    bool is_particle_active(const Particle& particle) const;

private:
    std::vector<std::unique_ptr<ITracerSink>> m_sinks;
    std::unordered_set<int> m_active_particles;
    mutable const Particle* m_last_particle = nullptr;
    mutable bool m_last_active = false;
};

inline bool Tracer::is_particle_active(const Particle& particle) const {
    if(m_last_particle != &particle) {
        m_last_active = m_active_particles.find(particle.id()) != m_active_particles.end(); 
        m_last_particle = &particle;
        return m_last_active;
    } else {
        return m_last_active;
    }
}
 

}

#else
#define SIM_TRACER_EVENT(tracer, type, sim, time)
#define PARTICLE_TRACER_EVENT(tracer, type, particle, sim, time)
#define PARTICLE_MOTION_TRACER_EVENT(tracer, type, particle, sim, time, dt, \
                 acceleration, update_fn) \
            update_fn();
#endif
 
#endif
