set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Grid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Particle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ParticleParameters.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PrototypalInteractionFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Simulation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SimulationTime.cpp
    PARENT_SCOPE)

if(ENABLE_TRACING)
    set(TRACING_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/TracerConfig.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tracing/FileSinkBackend.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tracing/ParticleFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tracing/PlotTracerFormatter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tracing/Tracer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tracing/TraceEvent.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tracing/TracerFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tracing/TracerFormatter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tracing/TracerSinkBackend.cpp
    PARENT_SCOPE)
else()
    set(TRACING_SOURCES PARENT_SCOPE)
endif()

if(ENABLE_COLOR_CUI)
    set(CUI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/terminal_ui/BasicGridPrinter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/terminal_ui/DensityPrinter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/terminal_ui/Palette.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/terminal_ui/ParticlePrinter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/terminal_ui/Terminal.cpp
    PARENT_SCOPE)
else()
    set(CUI_SOURCES PARENT_SCOPE)
endif()
