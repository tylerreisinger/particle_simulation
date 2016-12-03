#ifndef PS_COMMON_TYPES_H_
#define PS_COMMON_TYPES_H_

template<typename T>
class Vector2;

using PositionType = float;
using QuantityType = float;
using ChargeType = float;
using SpatialVector = Vector2<PositionType>;
using ForceType = SpatialVector;

#endif
