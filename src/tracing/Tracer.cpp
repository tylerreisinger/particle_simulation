#include "Tracer.h"

#include "TracerSink.h"
#include "../Particle.h"

namespace tracing {

Tracer::Tracer() {
 
}
 
Tracer::~Tracer() {
 
}
 
/*Tracer::Tracer(tracing::Tracer&& other) noexcept:
    m_sinks(std::move(other.m_sinks)) {
 
}
 
Tracer& Tracer::operator=(tracing::Tracer&& other) noexcept {
    m_sinks = std::move(other.m_sinks);
    return *this; 
}*/
 
void Tracer::broadcast_event(TraceEvent& event) const {
    for(auto& sink : m_sinks) {
        if(sink->is_enabled()) {
            sink->emit_record(event); 
        }
    } 
}
 
void Tracer::broadcast_event(TraceEvent&& event) const {
    for(auto& sink : m_sinks) {
        if(sink->is_enabled()) {
            sink->emit_record(event); 
        }
    } 
}
 
void Tracer::attach_sink(std::unique_ptr<ITracerSink> sink) {
    m_sinks.push_back(std::move(sink)); 
}
 
void Tracer::remove_all_sinks() {
    m_sinks.clear(); 
}
 
void Tracer::enable_particle_tracing(const Particle& particle) {
    enable_particle_tracing(particle.id());
}
void Tracer::enable_particle_tracing(int id) {
    m_active_particles.insert(id);
    m_last_particle = nullptr;
}
void Tracer::enable_particle_tracing(const std::vector<int>& ids) {
    for(auto id : ids) {
        enable_particle_tracing(id);
    }
}
void Tracer::enable_particle_tracing(std::initializer_list<int> ids) {
    for(auto id : ids) {
        enable_particle_tracing(id);
    }
}
 
void Tracer::disable_particle_tracing(const Particle& particle) {
    disable_particle_tracing(particle.id());
}
void Tracer::disable_particle_tracing(int id) {
    m_active_particles.erase(id); 
    m_last_particle = nullptr;
}
void Tracer::disable_particle_tracing(const std::vector<int>& ids) {
    for(auto id : ids) {
        disable_particle_tracing(id);
    }
}
void Tracer::disable_particle_tracing(std::initializer_list<int> ids) {
    for(auto id : ids) {
        disable_particle_tracing(id);
    }
}
}
