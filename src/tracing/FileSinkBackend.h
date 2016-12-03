#ifndef FILESINKBACKEND_H_
#define FILESINKBACKEND_H_

#include <fstream>

#include "TracerSinkBackend.h"

namespace tracing {

class FileSinkBackend: public ITracerSinkBackend<std::string> {
public:
    explicit FileSinkBackend(std::ofstream&& file);
    virtual ~FileSinkBackend() = default;

    FileSinkBackend(const FileSinkBackend& other) = delete;
    FileSinkBackend(FileSinkBackend&& other) = default;
    FileSinkBackend& operator =(const FileSinkBackend& other) = delete;
    FileSinkBackend& operator =(FileSinkBackend&& other) = default; 

    virtual void emit_record(const std::string& record) override;

private:    
    std::ofstream m_file; 
};

}

#endif
