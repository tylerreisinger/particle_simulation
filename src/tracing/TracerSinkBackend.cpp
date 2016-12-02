#include "TracerSinkBackend.h"

namespace tracing {


void ConsoleTracerBackend::emit_record(const std::string& record) {
    m_stream << record; 
}
 
}
