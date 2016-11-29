#ifndef PS_SIMULATION_TIME_H_
#define PS_SIMULATION_TIME_H_

#include <chrono>

class SimulationTime {
public:
    using Clock = std::chrono::high_resolution_clock;
    using ClockTimePoint = std::chrono::time_point<Clock>;
    using ClockDuration = std::chrono::duration<double, std::ratio<1, 1>>;
    using TimeType = double;

    SimulationTime();
    ~SimulationTime() = default;

    SimulationTime(const SimulationTime& other) = delete;
    SimulationTime(SimulationTime&& other) noexcept = default;
    SimulationTime& operator =(const SimulationTime& other) = delete;
    SimulationTime& operator =(SimulationTime&& other) noexcept = default;

    void begin_frame(TimeType timestep);

    ClockTimePoint previous_clock_time() const {return m_prev_clock_time;}
    ClockTimePoint current_clock_time() const {return m_cur_clock_time;}
    ClockDuration wall_time_elapsed() const {
        return std::chrono::duration_cast<ClockDuration>(
                m_cur_clock_time - m_prev_clock_time);
    }

    TimeType previous_simulation_time() const {return m_prev_frame_time;}
    TimeType current_simulation_time() const {return m_cur_frame_time;}
    TimeType time_delta() const {return m_dt;}

private:    
    ClockTimePoint m_prev_clock_time;
    ClockTimePoint m_cur_clock_time;

    TimeType m_prev_frame_time = 0.0;
    TimeType m_cur_frame_time = 0.0;
    TimeType m_dt = 0.0;
};

#endif
