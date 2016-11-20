#ifndef PS_VECTOR2_H_
#define PS_VECTOR2_H_

#include <utility>
#include <cmath>
#include <ostream>

#include "Angle.h"

template<typename T>
class Vector2
{
public:
    constexpr Vector2();
    ~Vector2() = default;

    constexpr Vector2(T x_val, T y_val);

    constexpr Vector2(const Vector2& lhs) = default;
    constexpr Vector2(Vector2&& lhs) noexcept = default;
    constexpr Vector2<T>& operator =(const Vector2<T>& rhs) = default;
    constexpr Vector2<T>& operator =(Vector2<T>&& rhs) noexcept = default;

    constexpr bool operator ==(const Vector2<T>& rhs) const;
    constexpr bool operator !=(const Vector2<T>& rhs) const;

    constexpr Vector2<T> operator +(Vector2<T> rhs) const;
    constexpr Vector2<T> operator -(Vector2<T> rhs) const;
    constexpr Vector2<T> operator *(T rhs) const;
    constexpr Vector2<T> operator /(T rhs) const;
    constexpr Vector2<T> operator -() const;

    Vector2<T>& operator +=(const Vector2<T>& rhs);
    Vector2<T>& operator -=(const Vector2<T>& rhs);
    Vector2<T>& operator *=(T rhs);
    Vector2<T>& operator /=(T rhs);

    constexpr T dot(const Vector2<T>& rhs) const;

    constexpr T magnitude_squared() const;
    T magnitude() const;

    Radians<T> angle_from_x_axis() const;

    template<typename U>
    friend constexpr Vector2<U> operator *(U lhs, Vector2<U> rhs);
    
    static constexpr Vector2<T> zero() {return Vector2<T>(T{0}, T{0});}
    static constexpr Vector2<T> unit_x() {return Vector2<T>(T{1}, T{0});}
    static constexpr Vector2<T> unit_y() {return Vector2<T>(T{0}, T{1});}

    T x;
    T y;
};

using Vector2d = Vector2<double>;
using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
using Vector2ui = Vector2<unsigned int>;

template<typename T>
void swap(Vector2<T>& lhs, Vector2<T>& rhs);

template<typename T>
std::ostream& operator <<(std::ostream& stream, const Vector2<T>& rhs)
{
    stream << "(" << rhs.x << ", " << rhs.y << ")";
    return stream;
}

template<typename T>
inline constexpr Vector2<T>::Vector2():
    x(T{0}), y(T{0})
{
}
 
template<typename T>
inline constexpr Vector2<T>::Vector2(T x_val, T y_val):
   x(x_val), y(y_val) 
{
}
 
template<typename T>
void swap(Vector2<T>& lhs, Vector2<T>& rhs)
{
    using std::swap;
    swap(lhs.x, rhs.x);
    swap(lhs.y, rhs.y);
}
 
template<typename T>
inline constexpr bool Vector2<T>::operator==(const Vector2<T>& rhs) const
{
    return (x == rhs.x && y == rhs.y); 
}
 
template<typename T>
inline constexpr bool Vector2<T>::operator!=(const Vector2<T>& rhs) const
{
    return !(*this == rhs); 
}
 
template<typename T>
inline constexpr Vector2<T> Vector2<T>::operator+(Vector2<T> rhs) const
{
    return Vector2<T>(x + rhs.x, y + rhs.y); 
}
 
template<typename T>
inline constexpr Vector2<T> Vector2<T>::operator-(Vector2<T> rhs) const
{
    return Vector2<T>(x - rhs.x, y - rhs.y); 
}
 
template<typename T>
inline constexpr Vector2<T> Vector2<T>::operator*(T rhs) const
{
    return Vector2<T>(x * rhs, y * rhs); 
}
 
template<typename T>
inline constexpr Vector2<T> Vector2<T>::operator/(T rhs) const
{
 
    return Vector2<T>(x / rhs, y / rhs); 
}
 
template<typename T>
inline constexpr Vector2<T> Vector2<T>::operator-() const
{
    return Vector2<T>(-x, -y); 
}

template<typename T>
constexpr Vector2<T> operator *(T lhs, Vector2<T> rhs)
{
    return rhs * lhs;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& rhs)
{
    x += rhs.x;
    y += rhs.y;    
    return *this;
}
 
template<typename T>
inline Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& rhs)
{
    x -= rhs.x;
    y -= rhs.y;    
    return *this;
}
 
template<typename T>
inline Vector2<T>& Vector2<T>::operator*=(T rhs)
{
    x *= rhs;
    y *= rhs;
    return *this; 
}
 
template<typename T>
inline Vector2<T>& Vector2<T>::operator/=(T rhs)
{
    x /= rhs;
    y /= rhs;
    return *this; 
}
 
template<typename T>
inline constexpr T Vector2<T>::dot(const Vector2<T>& rhs) const
{
    return x * rhs.x + y * rhs.y;
}
 
template<typename T>
inline constexpr T Vector2<T>::magnitude_squared() const
{
    return x*x + y*y; 
}
 
template<typename T>
inline T Vector2<T>::magnitude() const
{
    using std::sqrt;
    return sqrt(x*x + y*y); 
}
 
template<typename T>
inline Radians<T> Vector2<T>::angle_from_x_axis() const
{
    using std::atan2;
    return Radians<T>(atan2(y, x));
}

template<typename T, template <typename> class XDist, 
    template <typename> class YDist>
class Vector2Distribution {
public:
    Vector2Distribution(const XDist<T>& x_distribution, const YDist<T>& y_distribution):
        m_x_dist(x_distribution), m_y_dist(y_distribution) {
    }

    Vector2Distribution(XDist<T>&& x_distribution, YDist<T>&& y_distribution):
        m_x_dist(std::forward<XDist<T>>(x_distribution)), 
        m_y_dist(std::forward<YDist<T>>(y_distribution)) {
    }

    ~Vector2Distribution() = default;

    Vector2Distribution(const Vector2Distribution& other) = default;
    Vector2Distribution(Vector2Distribution&& other) noexcept = default;
    Vector2Distribution& operator =(const Vector2Distribution& other) = default;
    Vector2Distribution& operator =(Vector2Distribution&& other) noexcept = default;

    void reset() {
        m_x_dist.reset();
        m_y_dist.reset();
    }

    Vector2<T> min() const {
        return Vector2<T>(m_x_dist.min(), m_y_dist.min());
    }

    Vector2<T> max() const {
        return Vector2<T>(m_x_dist.max(), m_y_dist.max());
    }

    template<typename Rng>
    Vector2<T> operator()(Rng& rng) {
        return Vector2<T>(m_x_dist(rng), m_y_dist(rng));
    }

    XDist<T>& x_distribution() {
        return m_x_dist;
    }

    const XDist<T>& x_distribution() const {
        return m_x_dist;
    }

    YDist<T>& y_distribution() {
        return m_y_dist;
    }

    const YDist<T>& y_distribution() const {
        return m_y_dist;
    }

private:
    XDist<T> m_x_dist;
    YDist<T> m_y_dist;
};

template<typename T, template<typename> class XDist, 
    template <typename> class YDist>
auto make_vector2_distribution(XDist<T>&& x_distribution, YDist<T>&& y_distribution) {
    return Vector2Distribution<T, XDist, YDist>(std::forward<XDist<T>>(x_distribution),
            std::forward<YDist<T>>(y_distribution));
}

template<typename T, template<typename> class Dist>
auto make_vector2_distribution(const Dist<T>& component_distribution) {
    return Vector2Distribution<T, Dist, Dist>(component_distribution, 
            component_distribution);
}
 
#endif
