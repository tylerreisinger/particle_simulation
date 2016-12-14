#include "TracerConfig.h"

#include <memory>
#include <string>
#include <iostream>

#include "tracing/TracerSink.h"
#include "tracing/Tracer.h"
#include "tracing/PlotTracerFormatter.h"
#include "tracing/FileSinkBackend.h"
#include "tracing/ParticleFilter.h"

tracing::Tracer build_tracer() {
    tracing::Tracer tracer;

    auto cout_backend = std::make_shared<tracing::ConsoleTracerBackend>(std::cout);

    auto file_backend1 = std::make_shared<tracing::FileSinkBackend>(
        std::ofstream("particle_trace.txt"));
    auto file_backend2 = std::make_shared<tracing::FileSinkBackend>(
        std::ofstream("particle_trace2.txt"));
    auto file_backend3 = std::make_shared<tracing::FileSinkBackend>(
        std::ofstream("particle_trace3.txt"));

    auto formatter = std::make_shared<tracing::TextTracerFormatter>();

    auto plotting_formatter = std::make_shared<tracing::PlotTracerFormatter>();

    auto sink = std::make_unique<tracing::TracerSink<std::string>>(
            cout_backend, formatter);

    auto sink2 = std::make_unique<tracing::TracerSink<std::string>>(
            file_backend1, plotting_formatter, 
            std::make_shared<tracing::ParticleFilter>(0));

    auto sink3 = std::make_unique<tracing::TracerSink<std::string>>(
            file_backend2, plotting_formatter, 
            std::make_shared<tracing::ParticleFilter>(1));

    auto sink4 = std::make_unique<tracing::TracerSink<std::string>>(
            file_backend3, plotting_formatter, 
            std::make_shared<tracing::ParticleFilter>(2));

    tracer.attach_sink(std::move(sink));
    tracer.attach_sink(std::move(sink2));
    tracer.attach_sink(std::move(sink3));
    tracer.attach_sink(std::move(sink4));

    tracer.enable_particle_tracing({0, 1, 2});

    return tracer;
}
 
