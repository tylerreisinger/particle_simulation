#ifndef PS_DOUBLE_BUFFERED_H_
#define PS_DOUBLE_BUFFERED_H_

#include <utility>

template<typename T>
class DoubleBuffered {
public:
    DoubleBuffered();
    DoubleBuffered(const T& value);
    ~DoubleBuffered() = default;

    DoubleBuffered(const DoubleBuffered& other) = default;
    DoubleBuffered(DoubleBuffered&& other) noexcept = default;
    DoubleBuffered& operator =(const DoubleBuffered& other) = default;
    DoubleBuffered& operator =(DoubleBuffered&& other) noexcept = default;

    DoubleBuffered& operator =(const T& other);

    const T& get() const {return m_front;}
    void set(const T& value);
    void set(T&& value);
    void broadcast(const T& value);

    void apply_update();

private:
    T m_front;
    T m_back;
};

template<typename T>
inline DoubleBuffered<T>::DoubleBuffered(const T& value):
    m_front(value), m_back(value)
{}
 
template<typename T>
inline DoubleBuffered<T>& DoubleBuffered<T>::operator=(const T& other) {
    m_back = other;
    return *this; 
}
 
template<typename T>
inline void DoubleBuffered<T>::set(const T& value) {
    m_back = value; 
}
 
template<typename T>
inline void DoubleBuffered<T>::set(T&& value) {
    m_back = std::move(value); 
}
 
template<typename T>
inline void DoubleBuffered<T>::broadcast(const T& value) {
    m_front = value;
    m_back = value; 
}
 
template<typename T>
inline void DoubleBuffered<T>::apply_update() {
    m_front = m_back;
}
 
#endif
