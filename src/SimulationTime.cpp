#include "SimulationTime.h"


SimulationTime::SimulationTime() {
}
 
void SimulationTime::begin_frame(TimeType timestep) {
    m_prev_clock_time = m_cur_clock_time;
    m_cur_clock_time = Clock::now();
    
    m_prev_frame_time = m_cur_frame_time;
    m_cur_frame_time += timestep;
    m_dt = timestep;
}
 
