#ifndef TERMINAL_UI_VIEWPORT_H_
#define TERMINAL_UI_VIEWPORT_H_

#include "../Vector2.h"
#include "../CommonTypes.h"

class Viewport {
public:
    Viewport() = default;
    Viewport(PositionType left, PositionType top, PositionType right, PositionType bottom):
        m_top_left(left, top), m_bottom_right(right, bottom) {}

    ~Viewport() = default;

    Viewport(const Viewport& other) = default;
    Viewport(Viewport&& other) noexcept = default;
    Viewport& operator =(const Viewport& other) = default;
    Viewport& operator =(Viewport&& other) noexcept = default;

    const SpatialVector& top_left() const {return m_top_left;}
    const SpatialVector& bottom_right() const {return m_bottom_right;}

    PositionType x_min() const {return m_top_left.x;}
    PositionType x_max() const {return m_bottom_right.x;}
    PositionType y_min() const {return m_top_left.y;}
    PositionType y_max() const {return m_bottom_right.y;}

    bool is_within(const SpatialVector& position) const {
        return (position.x >= x_min() && position.x < x_max()
                && position.y >= y_min() && position.y < y_max());
    }

private:
   SpatialVector m_top_left;
   SpatialVector m_bottom_right; 
};


#endif
