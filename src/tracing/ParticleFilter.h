#ifndef PARTICLEFILTER_H_
#define PARTICLEFILTER_H_

#include <unordered_set>
#include <vector>
#include <initializer_list>

#include "TracerFilter.h"
#include "TraceEvent.h"

namespace tracing {

class ParticleFilter: public TracerFilter,
                      public TraceEventVisitor {
public:
    explicit ParticleFilter(std::initializer_list<int> enabled_particles);
    explicit ParticleFilter(int enabled_particle);
    explicit ParticleFilter(const std::vector<int>& enabled_particles);
    ~ParticleFilter() = default;

    ParticleFilter(const ParticleFilter& other) = delete;
    ParticleFilter(ParticleFilter&& other) noexcept = default;
    ParticleFilter& operator =(const ParticleFilter& other) = delete;
    ParticleFilter& operator =(ParticleFilter&& other) noexcept = default;

    virtual bool filter(TraceEvent& event) override {
        event.accept(*this);
        return m_result;
    }        

    virtual void visit(TraceEvent& event) override {
        //Not associated with a particle, filter out.
        m_result = false;
    }

    virtual void visit(ParticleTraceEvent& event) override {
        auto it = m_enabled_particles.find(event.particle_id());
        m_result = (it != m_enabled_particles.end());
    }

    virtual void visit(ParticleMotionTraceEvent& event) override {
        visit(static_cast<ParticleTraceEvent&>(event));
    }

private:
    bool m_result;
    std::unordered_set<int> m_enabled_particles;
};

}

#endif
