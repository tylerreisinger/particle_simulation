#ifndef PARTICLEPARAMETERS_H_
#define PARTICLEPARAMETERS_H_

#include <vector>

#include <boost/optional.hpp>

#include "Vector2.h"
#include "CommonTypes.h"

class ParticleParameters {
public:
    ParticleParameters() = default;

    ParticleParameters(const boost::optional<SpatialVector>& position,
            const boost::optional<SpatialVector>& velocity = boost::none,
            boost::optional<QuantityType> radius = boost::none,
            boost::optional<QuantityType> mass = boost::none,
            std::vector<boost::optional<QuantityType>> charges = {}
            );

    ~ParticleParameters() = default;

    ParticleParameters(const ParticleParameters& other) = default;
    ParticleParameters(ParticleParameters&& other) noexcept = default;
    ParticleParameters& operator =(const ParticleParameters& other) = default;
    ParticleParameters& operator =(ParticleParameters&& other) noexcept = default;

    const boost::optional<SpatialVector>& position() const {return m_position;}
    const boost::optional<SpatialVector>& velocity() const {return m_velocity;}
    const boost::optional<QuantityType>& mass() const {return m_mass;}
    const boost::optional<QuantityType>& radius() const {return m_radius;}
    const std::vector<boost::optional<ChargeType>>& charges() const {return m_charges;}

    ParticleParameters& set_position(const SpatialVector& value);
    ParticleParameters& set_velocity(const SpatialVector& value);
    ParticleParameters& set_radius(QuantityType value);
    ParticleParameters& set_mass(QuantityType value);
    ParticleParameters& set_charge(std::size_t idx, ChargeType value);
    ParticleParameters& set_charges(std::initializer_list<ChargeType> charges);

    ParticleParameters& unset_position();
    ParticleParameters& unset_velocity();
    ParticleParameters& unset_radius();
    ParticleParameters& unset_mass();
    ParticleParameters& unset_charge(std::size_t idx);
    ParticleParameters& unset_all_charges();

private: 
    boost::optional<SpatialVector> m_position = boost::none;    
    boost::optional<SpatialVector> m_velocity = boost::none;    
    boost::optional<QuantityType> m_radius = boost::none;
    boost::optional<QuantityType> m_mass = boost::none;
    std::vector<boost::optional<ChargeType>> m_charges;
};

#endif
