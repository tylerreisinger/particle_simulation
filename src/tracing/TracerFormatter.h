#ifndef PS_TRACERFORMATTER_H_
#define PS_TRACERFORMATTER_H_

#include <functional>
#include <boost/optional.hpp>

#include "TraceEvent.h"

namespace tracing {

template <typename RecordFormat>
class TracerFormatter {
public:
    TracerFormatter() {};
    virtual ~TracerFormatter() {};

    TracerFormatter(const TracerFormatter& other) = delete;
    TracerFormatter(TracerFormatter&& other) noexcept = default;
    TracerFormatter& operator =(const TracerFormatter& other) = delete;
    TracerFormatter& operator =(TracerFormatter&& other) noexcept = default;

    virtual boost::optional<RecordFormat> format_record(TraceEvent& event) = 0;
};

class TextTracerFormatter: public TracerFormatter<std::string>,
                           public TraceEventVisitor {
public:
    using RecordFormat = std::string;

    TextTracerFormatter() {};
    virtual ~TextTracerFormatter() {};

    virtual boost::optional<std::string> format_record(TraceEvent& event) override;

    virtual void visit(TraceEvent& event) override;
    virtual void visit(ParticleTraceEvent& event) override;
    virtual void visit(ParticleMotionTraceEvent& event) override;

private:
    void indent(std::stringstream& stream);


    std::string m_record;
    int m_nest_depth = 0;
};

}

#endif
