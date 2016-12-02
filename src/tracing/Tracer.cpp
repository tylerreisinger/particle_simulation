#include "Tracer.h"

#include "TracerSink.h"
#include "../Particle.h"

namespace tracing {

Tracer::Tracer() {
 
}
 
Tracer::~Tracer() {
 
}
 
Tracer::Tracer(tracing::Tracer&& other) noexcept:
    m_sinks(std::move(other.m_sinks)) {
 
}
 
Tracer& Tracer::operator=(tracing::Tracer&& other) noexcept {
    m_sinks = std::move(other.m_sinks);
    return *this; 
}
 
void Tracer::broadcast_event(TraceEvent& event) {
    for(auto& sink : m_sinks) {
        sink->emit_record(event); 
    } 
}
 
void Tracer::broadcast_event(TraceEvent&& event) {
    for(auto& sink : m_sinks) {
        sink->emit_record(event); 
    } 
}
 
void Tracer::attach_sink(std::unique_ptr<ITracerSink> sink) {
    m_sinks.push_back(std::move(sink)); 
}
 
void Tracer::remove_all_sinks() {
    m_sinks.clear(); 
}
 
void Tracer::enable_particle_tracing(const Particle& particle) {
    m_active_particles.insert(particle.id());
}
 
void Tracer::disable_particle_tracing(const Particle& particle) {
    m_active_particles.erase(particle.id()); 
}
 
}
