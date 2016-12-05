#ifndef TERMINAL_UI_PALETTE_H_
#define TERMINAL_UI_PALETTE_H_

#include <vector>
#include <initializer_list>

#include "Terminal.h"

namespace tui {

class Palette {
public:
    using iterator = std::vector<term::TerminalColor>::iterator;
    using const_iterator = std::vector<term::TerminalColor>::const_iterator;

    Palette() = default;
    Palette(std::vector<term::TerminalColor> colors);
    Palette(std::initializer_list<term::TerminalColor> colors);
    ~Palette() = default;

    Palette(const Palette& other) = default;
    Palette(Palette&& other) noexcept = default;
    Palette& operator =(const Palette& other) = default;
    Palette& operator =(Palette&& other) noexcept = default;

    term::TerminalColor& operator [](std::size_t idx) {
        return get_color(idx);
    }
    const term::TerminalColor& operator [](std::size_t idx) const {
        return get_color(idx);
    }

    const_iterator begin() const {
        return m_colors.begin();
    }
    const_iterator end() const {
        return m_colors.end();
    }
    iterator begin() {
        return m_colors.begin();
    }
    iterator end() {
        return m_colors.end();
    }

    std::size_t color_count() const {
        return m_colors.size();
    }

    term::TerminalColor& get_color(std::size_t idx) {
        idx = idx % color_count();
        return m_colors[idx];
    }
    const term::TerminalColor& get_color(std::size_t idx) const {
        idx = idx % color_count();
        return m_colors[idx];
    }

    void append_color(term::TerminalColor color);
    
private:
    std::vector<term::TerminalColor> m_colors;
};

}

#endif
