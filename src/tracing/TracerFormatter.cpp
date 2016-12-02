#include "TracerFormatter.h"

#include <sstream>

namespace tracing {

std::string TextTracerFormatter::format_record(TraceEvent& event) {
    event.accept(*this); 
    return m_record;
}
 
void TextTracerFormatter::visit(TraceEvent& event) {
    std::stringstream record_stream;
    switch(event.type()) {
    case TraceEventType::FrameBegin:
        record_stream << "===== Frame start, t=" 
            << event.simulation_time().current_simulation_time() << " =====\n";
        break;
    case TraceEventType::FrameEnd:
        break;
    default:
        break;
    } 

    m_record = record_stream.str();
}
 
void TextTracerFormatter::visit(ParticleTraceEvent& event) {
    std::stringstream record_stream;
    switch(event.type()) {
    case TraceEventType::ParticleFrameBegin:
        record_stream << "Particle #" << event.particle_id() << ": \n";
        m_nest_depth += 1;
        break;
    case TraceEventType::ParticleFrameEnd:
        m_nest_depth -= 1;
        break;
    case TraceEventType::WallCollideBegin:
        indent(record_stream);
        record_stream << "Wall collision\n";
        m_nest_depth += 1;
        break;
    case TraceEventType::WallCollideEnd:
        m_nest_depth -= 1;
        break;
    default:
        break;
    } 

    m_record = record_stream.str();
}
 
void TextTracerFormatter::visit(tracing::ParticleMotionTraceEvent& event) {
    std::stringstream record_stream;
    switch(event.type()) {
    case TraceEventType::MotionParamsUpdated: 
        indent(record_stream);
        record_stream << "dt=" << event.update_dt() << "; pos="
            << event.starting_position() << "->" << event.ending_position()
            << "; v=" << event.starting_velocity() << "->" << event.ending_velocity()
            << "; a=" << event.acceleration() << "\n";
        break;
    default:
        break;
    } 

    m_record = record_stream.str();
}
 
void TextTracerFormatter::indent(std::stringstream& stream) {
    for(int i = 0; i < m_nest_depth; ++i) {
        stream << "  ";
    }
}
 
}
