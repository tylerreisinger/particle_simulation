#ifndef PS_TRACERSINKBACKGROUND_H_
#define PS_TRACERSINKBACKGROUND_H_

#include <iostream>
#include <string>

namespace tracing {

template <typename RecordType>
class ITracerSinkBackend {
public:
    ITracerSinkBackend() = default;
    virtual ~ITracerSinkBackend() {};

    virtual void emit_record(const RecordType& record) = 0;
};

class ConsoleTracerBackend: public ITracerSinkBackend<std::string> {
public:
    using RecordType = std::string;

    explicit ConsoleTracerBackend(std::ostream& stream = std::cout):
        m_stream(stream) {}

    virtual ~ConsoleTracerBackend() {};

    ConsoleTracerBackend(const ConsoleTracerBackend& other) = delete;
    ConsoleTracerBackend(ConsoleTracerBackend&& other) noexcept = default;
    ConsoleTracerBackend& operator =(const ConsoleTracerBackend& other) = delete;
    ConsoleTracerBackend& operator =(ConsoleTracerBackend&& other) noexcept = default;

    virtual void emit_record(const RecordType& record) override;

private:
    std::ostream& m_stream;
};

}

#endif
