#include "SimulationRunner.h"

#include <thread>

#include "Simulation.h"



SimulationRunner::SimulationRunner(std::unique_ptr<Simulation> simulation):
    m_simulation(std::move(simulation)) {
}
 
SimulationRunner::~SimulationRunner() {
}


const SimulationTime& SimulationRunner::simulation_time() const {
    return m_simulation->simulation_time(); 
}
 
SimulationRunner& SimulationRunner::set_stopping_time(double end_time) {
    m_stop_time = end_time;
    return *this;
}
 
SimulationRunner& SimulationRunner::set_stopping_condition(StoppingFn fn) {
    m_stopping_condition = std::move(fn); 
    return *this;
}
 
SimulationRunner& SimulationRunner::set_stopping_condition(std::nullptr_t) {
    m_stopping_condition = nullptr;
    return *this; 
}
 
void SimulationRunner::run() {
    m_is_running = true;
    while(m_is_running) {
        step();
        if(m_stopping_condition) {
            m_is_running = m_stopping_condition(*m_simulation, *this);
        }
        if(m_stop_time <= m_simulation->simulation_time().current_simulation_time()) {
            m_is_running = false;
        }
        if(m_delay.count() != 0.0) {
            std::this_thread::sleep_for(m_delay);
        }
    }     
}
 
void SimulationRunner::step() {
    m_on_frame_start_event(*m_simulation, *this);
    execute_frame(); 
    m_on_frame_end_event(*m_simulation, *this);
}
 
void SimulationRunner::pause() {
    m_is_running = false; 
}
 
void SimulationRunner::execute_frame() {
    m_simulation->do_frame();
}
 
