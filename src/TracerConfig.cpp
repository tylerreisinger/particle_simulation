#include "TracerConfig.h"

#include <memory>
#include <string>
#include <iostream>

#include "tracing/TracerSink.h"
#include "tracing/Tracer.h"
#include "tracing/PlotTracerFormatter.h"
#include "tracing/FileSinkBackend.h"

tracing::Tracer build_tracer() {
    tracing::Tracer tracer;

    auto cout_backend = std::make_shared<tracing::ConsoleTracerBackend>(std::cout);
    auto file_backend = std::make_shared<tracing::FileSinkBackend>(
        std::ofstream("particle_trace.txt"));

    auto formatter = std::make_shared<tracing::TextTracerFormatter>();

    auto plotting_formatter = std::make_shared<tracing::PlotTracerFormatter>();

    auto sink = std::make_unique<tracing::TracerSink<std::string>>(
            cout_backend, formatter);

    auto sink2 = std::make_unique<tracing::TracerSink<std::string>>(
            file_backend, plotting_formatter);

    tracer.attach_sink(std::move(sink));
    tracer.attach_sink(std::move(sink2));

    return tracer;
}
 
