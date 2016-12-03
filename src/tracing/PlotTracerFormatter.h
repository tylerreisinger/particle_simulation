#ifndef PS_PLOTTRACERFORMATTER_H_
#define PS_PLOTTRACERFORMATTER_H_

#include "TracerFormatter.h"

namespace tracing {

class PlotTracerFormatter: public TracerFormatter<std::string>,
                           public TraceEventVisitor {
public:
    enum class Column {
        ParticleId,
        PosX,
        PosY,
        VelX,
        VelY,
        Acceleration
    };

    
    PlotTracerFormatter() = default;
    ~PlotTracerFormatter() = default;

    PlotTracerFormatter(const PlotTracerFormatter& other) = delete;
    PlotTracerFormatter(PlotTracerFormatter&& other) noexcept = default;
    PlotTracerFormatter& operator =(const PlotTracerFormatter& other) = delete;
    PlotTracerFormatter& operator =(PlotTracerFormatter&& other) noexcept = default;

    virtual boost::optional<std::string> format_record(TraceEvent& event) override;

    virtual void visit(ParticleMotionTraceEvent& event) override;

private: 
    boost::optional<std::string> m_record;
};

}

#endif
