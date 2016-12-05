#include "Palette.h"

namespace tui {

Palette::Palette(std::vector<term::TerminalColor> colors):
    m_colors(std::move(colors)) {
 
}

Palette::Palette(std::initializer_list<term::TerminalColor> colors):
    m_colors(std::move(colors)) {
 
}
 
void Palette::append_color(term::TerminalColor color) {
    m_colors.emplace_back(std::move(color)); 
}
 
}
