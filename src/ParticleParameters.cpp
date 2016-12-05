#include "ParticleParameters.h"

ParticleParameters::ParticleParameters(const boost::optional<SpatialVector>& position,
        const boost::optional<SpatialVector>& velocity,
        boost::optional<QuantityType> radius,
        boost::optional<QuantityType> mass,
        std::vector<boost::optional<QuantityType>> charges):
    m_position(position), m_velocity(velocity), m_radius(radius),
    m_mass(mass), m_charges(std::move(charges)) {
    
}

ParticleParameters& ParticleParameters::set_position(const SpatialVector& value) {
    m_position = value; 
    return *this;
}
 
ParticleParameters& ParticleParameters::set_velocity(const SpatialVector& value) {
    m_velocity = value;
    return *this; 
}
 
ParticleParameters& ParticleParameters::set_radius(QuantityType value) {
    m_radius = value;
    return *this; 
}
 
ParticleParameters& ParticleParameters::set_mass(QuantityType value) {
    m_mass = value;
    return *this;
}
 
ParticleParameters& ParticleParameters::set_charge(std::size_t idx, ChargeType value) {
    if(idx >= m_charges.size()) {
        m_charges.resize(idx+1);
    }
    m_charges[idx] = value; 
    return *this;
}
 
ParticleParameters& ParticleParameters::set_charges(
        std::initializer_list<ChargeType> charges) {
    m_charges.clear();
    m_charges.resize(charges.size());
    std::copy(charges.begin(), charges.end(), m_charges.begin());
    return *this;
}
 
ParticleParameters& ParticleParameters::unset_position() {
    m_position = boost::none;
    return *this; 
}
 
ParticleParameters& ParticleParameters::unset_velocity() {
    m_velocity = boost::none;
    return *this; 
}
 
ParticleParameters& ParticleParameters::unset_radius() {
    m_radius = boost::none;
    return *this;    
}
 
ParticleParameters& ParticleParameters::unset_mass() {
    m_mass = boost::none;
    return *this; 
}
 
ParticleParameters& ParticleParameters::unset_charge(std::size_t idx) {
    if(idx < m_charges.size()) {
        m_charges[idx] = boost::none;
    }
    return *this;
}
 
ParticleParameters& ParticleParameters::unset_all_charges() {
    m_charges.clear(); 
    return *this;
}
 
