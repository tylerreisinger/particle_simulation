#ifndef PS_TRACERFILTER_H_
#define PS_TRACERFILTER_H_

#include <functional>

#include "TraceEvent.h"

namespace tracing {

class TracerFilter {
public:
    TracerFilter() = default;
    virtual ~TracerFilter() {};

    TracerFilter(const TracerFilter& other) = delete;
    TracerFilter(TracerFilter&& other) noexcept = default;
    TracerFilter& operator =(const TracerFilter& other) = delete;
    TracerFilter& operator =(TracerFilter&& other) noexcept = default;

    virtual bool filter(TraceEvent& event) {return true;}
};

class FunctionTracerFilter: public TracerFilter,
                            public TraceEventVisitor {
public:
    FunctionTracerFilter();
    virtual ~FunctionTracerFilter();

    FunctionTracerFilter(FunctionTracerFilter&& other) = default;
    FunctionTracerFilter& operator =(FunctionTracerFilter&& other) = default;

    virtual bool filter(TraceEvent& event) override {
        event.accept(*this);
        return m_result;
    }

    virtual void visit(TraceEvent& event) override {
        m_result = m_fn1(event);
    }

    virtual void visit(ParticleTraceEvent& event) override {
        m_result = m_fn2(event);
    }

private:
    std::function<bool (TraceEvent& evt)> m_fn1 = [](TraceEvent&) {return false;};
    std::function<bool (ParticleTraceEvent& evt)> m_fn2 = 
        [](ParticleTraceEvent&) {return false;};

    bool m_result = false;
};


}

#endif
