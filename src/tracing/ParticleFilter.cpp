#include "ParticleFilter.h"

namespace tracing {

ParticleFilter::ParticleFilter(std::initializer_list<int> enabled_particles):
    m_enabled_particles(enabled_particles) {
     
}
 
ParticleFilter::ParticleFilter(int enabled_particle) {
    m_enabled_particles.insert(enabled_particle);
}

ParticleFilter::ParticleFilter(const std::vector<int>& enabled_particles) {
    m_enabled_particles.reserve(enabled_particles.size());
    for(auto id : enabled_particles) {
        m_enabled_particles.insert(id);
    }
}
 
}
