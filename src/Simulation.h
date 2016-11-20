#ifndef PS_SIMULATION_H_
#define PS_SIMULATION_H_

class Simulation {
public:
    Simulation() = delete;
    ~Simulation() = default;

    Simulation(const Simulation& other) = delete;
    Simulation(Simulation&& other) noexcept = delete;
    Simulation& operator =(const Simulation& other) = delete;
    Simulation& operator =(Simulation&& other) noexcept = delete;

   
private: 

};

#endif
