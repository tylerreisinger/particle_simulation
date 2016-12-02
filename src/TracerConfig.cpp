#include "TracerConfig.h"

#include <memory>
#include <string>
#include <iostream>

#include "tracing/TracerSink.h"

tracing::Tracer build_tracer() {
    tracing::Tracer tracer;

    auto backend = std::make_shared<tracing::ConsoleTracerBackend>(std::cout);
    auto formatter = std::make_shared<tracing::TextTracerFormatter>();

    auto sink = std::make_unique<tracing::TracerSink<std::string>>(
            backend, formatter);

    tracer.attach_sink(std::move(sink));

    return tracer;
}
 
