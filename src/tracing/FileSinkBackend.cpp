#include "FileSinkBackend.h"

namespace tracing {

FileSinkBackend::FileSinkBackend(std::ofstream&& file):
    m_file(std::move(file)) {
}
 
void FileSinkBackend::emit_record(const std::string& record) {
    m_file << record; 
}
 
}
