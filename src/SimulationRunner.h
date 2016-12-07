#ifndef SIMULATIONRUNNER_H_
#define SIMULATIONRUNNER_H_

#include <memory>
#include <limits>
#include <functional>
#include <chrono>

#include "SimulationTime.h"
#include "Event.h"

class Simulation;

class SimulationRunner {
public:
    using StoppingFn = std::function<bool (const Simulation&, SimulationRunner&)>;
    using FrameEvent = Event<void (Simulation&, SimulationRunner&)>;
    using FrameEventFn = FrameEvent::FunctionType;

    SimulationRunner(std::unique_ptr<Simulation> simulation);
    ~SimulationRunner();

    SimulationRunner(const SimulationRunner& other) = delete;
    SimulationRunner(SimulationRunner&& other) = default;
    SimulationRunner& operator =(const SimulationRunner& other) = delete;
    SimulationRunner& operator =(SimulationRunner&& other) = default;

    Simulation& simulation() {return *m_simulation;}
    const Simulation& simulation() const {return *m_simulation;}
    const SimulationTime& simulation_time() const;

    double get_stopping_time() const {return m_stop_time;}
    SimulationRunner& set_stopping_time(double end_time);
    SimulationRunner& set_stopping_condition(StoppingFn fn);
    SimulationRunner& set_stopping_condition(std::nullptr_t fn);

    template <typename Duration>
    SimulationRunner& set_delay(const Duration& duration) {
        m_delay = std::chrono::duration_cast<
            std::chrono::duration<double, std::ratio<1, 1>>>(duration);
        return *this;
    }
    std::chrono::duration<double, std::ratio<1, 1>> delay() const {
        return m_delay;
    }

    EventConnection on_frame_start(FrameEventFn fn) {
        return m_on_frame_start_event.register_handler(std::move(fn));
    }
    EventConnection on_frame_end(FrameEventFn fn) {
        return m_on_frame_end_event.register_handler(std::move(fn));
    }

    void run();
    void step();
    void pause();
    
private:
    void execute_frame();

    std::unique_ptr<Simulation> m_simulation;
    StoppingFn m_stopping_condition;
    FrameEvent m_on_frame_start_event;
    FrameEvent m_on_frame_end_event;

    std::chrono::duration<double, std::ratio<1, 1>> m_delay 
        = std::chrono::duration<double, std::ratio<1, 1>>(0.0);
    double m_stop_time = std::numeric_limits<double>::max();
    bool m_is_running = false;
};

#endif
