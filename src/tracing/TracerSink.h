#ifndef PS_TRACERSINK_H_
#define PS_TRACERSINK_H_

#include <memory>
#include <cassert>

#include "TraceEvent.h"
#include "TracerFormatter.h"
#include "TracerFilter.h"
#include "TracerSinkBackend.h"

namespace tracing {

class ITracerSink {
public:
    ITracerSink() = default;
    virtual ~ITracerSink() {};

    virtual void emit_record(TraceEvent& event) = 0;
};

template <typename RecordType>
class TracerSink: public ITracerSink {
public:
    TracerSink(std::shared_ptr<ITracerSinkBackend<RecordType>> backend,
            std::shared_ptr<TracerFormatter<RecordType>> formatter);
    virtual ~TracerSink() {};

    TracerSink(const TracerSink& other) = delete;
    TracerSink(TracerSink&& other) noexcept = default;
    TracerSink& operator =(const TracerSink& other) = delete;
    TracerSink& operator =(TracerSink&& other) noexcept = default;

    virtual void emit_record(TraceEvent& event) override;

    TracerSink& set_filter(std::shared_ptr<TracerFilter>& filter);
    TracerSink& set_formatter(
            std::shared_ptr<TracerFormatter<RecordType>>& formatter);
    TracerSink& set_backend(
            std::shared_ptr<ITracerSinkBackend<RecordType>>& backend);

private:
    std::shared_ptr<TracerFilter> m_filter = 
        std::make_shared<TracerFilter>();
    std::shared_ptr<ITracerSinkBackend<RecordType>> m_backend;
    std::shared_ptr<TracerFormatter<RecordType>> m_formatter;
};

template<typename RecordType>
inline TracerSink<RecordType>::TracerSink(
        std::shared_ptr<ITracerSinkBackend<RecordType>> backend,
        std::shared_ptr<TracerFormatter<RecordType>> formatter):
    m_backend(std::move(backend)), m_formatter(std::move(formatter)) {
 
}
 
template<typename RecordType>
inline void TracerSink<RecordType>::emit_record(TraceEvent& event) {
    assert(m_formatter != nullptr && "Tracer sink needs a valid formatter");
    assert(m_backend != nullptr && "Tracer sink needs a valid backend");

    if(m_filter->filter(event)) {
        auto formatted_record = m_formatter->format_record(event);
        if(formatted_record) {
            m_backend->emit_record(formatted_record.value());
        }
    } 
}
 
template<typename RecordType>
inline TracerSink<RecordType>& TracerSink<RecordType>::set_filter(
        std::shared_ptr<TracerFilter>& filter) {
    m_filter = filter; 
    return *this;
}
 
template<typename RecordType>
inline TracerSink<RecordType>& TracerSink<RecordType>::set_formatter(
        std::shared_ptr<TracerFormatter<RecordType>>& formatter) {
    m_formatter = formatter; 
    return *this;
}
 
template<typename RecordType>
inline TracerSink<RecordType>& TracerSink<RecordType>::set_backend(
        std::shared_ptr<ITracerSinkBackend<RecordType>>& backend) {
    m_backend = backend; 
    return *this;
}

}

#endif
