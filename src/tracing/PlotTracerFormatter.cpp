#include "PlotTracerFormatter.h"

#include <sstream>

namespace tracing {

boost::optional<std::string> PlotTracerFormatter::format_record(TraceEvent& event) {
    m_record = boost::none;
    event.accept(*this);
    return m_record; 
}
 
void PlotTracerFormatter::visit(ParticleMotionTraceEvent& event) {
    switch(event.type()) {
    case TraceEventType::MotionParamsUpdated: {
        std::ostringstream record_stream;
        record_stream << event.particle_id() << "\t"
            << event.simulation_time().current_simulation_time() << "\t" 
            << event.update_dt() << "\t" << event.ending_position().x << "\t"
            << event.ending_position().y << "\t" << event.ending_velocity().x << "\t"
            << event.ending_velocity().y << "\t" << event.acceleration().x << "\t"
            << event.acceleration().y << "\n";
        m_record = record_stream.str();
        break;
    }
    default:
        break;
    }
}
 
}
