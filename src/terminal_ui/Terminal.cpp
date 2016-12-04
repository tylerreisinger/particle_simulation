#include "Terminal.h"

#include <vector>
#include <string>

namespace term {

std::unique_ptr<Terminal> Terminal::m_instance = nullptr;

void Terminal::clear() {
    std::cout << "\033[2J";
}
 
void Terminal::set_underline(bool enable) {
    if(enable) {
        std::cout << "\033[4m"; 
    } else {
        std::cout << "\033[24m";
    }
    m_format.attributes().set_underlined(enable);
}
 
void Terminal::set_bold(bool enable) {
    if(enable) {
        set_font_weight(FontWeight::Bold);
    } else {
        set_font_weight(FontWeight::Normal);
    }
    m_format.attributes().set_bold(enable);
}
 
void Terminal::set_faint(bool enable) {
    if(enable) {
        set_font_weight(FontWeight::Faint);
    } else {
        set_font_weight(FontWeight::Normal);
    } 
    m_format.attributes().set_faint(enable);
}
 
void Terminal::set_font_weight(FontWeight mode) {
    switch(mode) {
    case FontWeight::Normal:
        std::cout << "\033[22m";
        break;
    case FontWeight::Bold:
        std::cout << "\033[1m";
        break;
    case FontWeight::Faint:
        std::cout << "\033[2m";
        break;
    }
    m_format.attributes().set_weight(mode);
}
 
void Terminal::set_italic(bool enable) {
    if(enable) {
        std::cout << "\033[3m";
    } else {
        std::cout << "\033[23m";
    }
    m_format.attributes().set_italic(enable);
}
 
void Terminal::set_strikethrough(bool enable) {
    if(enable) {
        std::cout << "\033[9m";
    } else {
        std::cout << "\033[29m";
    } 
    m_format.attributes().set_strikethrough(enable);
}
 
void Terminal::set_blink(BlinkMode mode) {
    switch(mode) {
    case BlinkMode::Off:
        std::cout << "\033[5m";
        break;
    case BlinkMode::Slow:
        std::cout << "\033[6m";
        break;
    case BlinkMode::Fast:
        std::cout << "\033[25m";
    } 
    m_format.attributes().set_blink(mode);
}
 
void Terminal::set_negative(bool enable) {
    if(enable) {
        std::cout << "\033[7m";
    } else {
        std::cout << "\033[27m";
    }
    m_format.attributes().set_negative(enable);
}
 
void Terminal::set_hidden(bool enable) {
    if(enable) {
        std::cout << "\033[8m";
    } else {
        std::cout << "\033[28m";
    } 
    m_format.attributes().set_hidden(enable);
}
 
void Terminal::reset_formatting() {
    std::cout << "\033[0m"; 
    m_format.set_attributes(TerminalAttributes());
}
 
void Terminal::set_foreground_color(const Color16& color) {
    if(color.palette_index() < 8) {
        std::cout << "\033[" << 30+color.palette_index() << "m";
    } else {
        std::cout << "\033[" << 90+(color.palette_index()-8) << "m";
    } 
    m_format.set_foreground_color(make_terminal_color(color));
}
 
void Terminal::set_background_color(const Color16& color) {
    if(color.palette_index() < 8) {
        std::cout << "\033[" << 40+color.palette_index() << "m";
    }  else {
        std::cout << "\033[" << 100+(color.palette_index()-8) << "m";
    }
    m_format.set_background_color(make_terminal_color(color));
}

void Terminal::set_foreground_color(const Color256& color) {
    std::cout << "\033[38;5;" << color.palette_index() << "m"; 
    m_format.set_foreground_color(make_terminal_color(color));
}
 
void Terminal::set_background_color(const Color256& color) {
    std::cout << "\033[48;5;" << color.palette_index() << "m"; 
    m_format.set_background_color(make_terminal_color(color));
}
 
void Terminal::set_foreground_color(const Rgb& color) {
    std::cout << "\033[38;2;" << +color.red() << ";"
       << +color.green() << ";" << +color.blue() << "m"; 
    m_format.set_foreground_color(make_terminal_color(color));
}
 
void Terminal::set_background_color(const Rgb& color) {
    std::cout << "\033[48;2;" << +color.red() << ";"
       << +color.green() << ";" << +color.blue() << "m"; 
    m_format.set_background_color(make_terminal_color(color));
}
 
void Terminal::set_foreground_color(const TerminalColor& color) {
    color.apply_foreground(*this);
}

void Terminal::set_background_color(const TerminalColor& color) {
    color.apply_background(*this);     
}

void Terminal::set_attributes(const TerminalAttributes& attributes) {
    set_underline(attributes.is_underlined());
    set_font_weight(attributes.weight());
    set_italic(attributes.is_italic());
    set_strikethrough(attributes.is_strikethrough());
    set_blink(attributes.blink());
    set_negative(attributes.is_negative());
    set_hidden(attributes.is_hidden());
}

void Terminal::set_formatting(const TerminalFormatting& formatting) {
    set_attributes(formatting.attributes());
    set_foreground_color(formatting.foreground_color());
    set_background_color(formatting.background_color()); 
}
 
Rgb::Rgb(uint8_t red, uint8_t green, uint8_t blue):
    m_red(red), m_green(green), m_blue(blue) {
}
 
void Color16::apply_foreground(Terminal& terminal) const {
    terminal.set_foreground_color(*this); 
}
 
void Color16::apply_background(Terminal& terminal) const {
    terminal.set_background_color(*this); 
}
 
void Color256::apply_foreground(Terminal& terminal) const {
    terminal.set_foreground_color(*this); 
}

void Color256::apply_background(Terminal& terminal) const {
    terminal.set_background_color(*this); 
}
 
void Rgb::apply_foreground(Terminal& terminal) const {
    terminal.set_foreground_color(*this); 
}
 
void Rgb::apply_background(Terminal& terminal) const {
    terminal.set_background_color(*this); 
}
 

std::ostream& operator<<(std::ostream& stream, ColorPalette16 color) {
    switch(color) {
    case ColorPalette16::Black:
        stream << "Black";
        break;
    case ColorPalette16::Red:
        stream << "Red";
        break;
    case ColorPalette16::Green:
        stream << "Green";
        break;
    case ColorPalette16::Yellow:
        stream << "Yellow";
        break;
    case ColorPalette16::Blue:
        stream << "Blue";
        break;
    case ColorPalette16::Magenta:
        stream << "Magenta";
        break;
    case ColorPalette16::Cyan:
        stream << "Cyan";
        break;
    case ColorPalette16::DullWhite:
        stream << "DullWhite";
        break;
    case ColorPalette16::Gray:
        stream << "Gray";
        break;
    case ColorPalette16::BrightRed:
        stream << "BrightRed";
        break;
    case ColorPalette16::BrightGreen:
        stream << "BrightGreen";
        break;
    case ColorPalette16::BrightYellow:
        stream << "BrightYellow";
        break;
    case ColorPalette16::BrightBlue:
        stream << "BrightBlue";
        break;
    case ColorPalette16::BrightMagenta:
        stream << "BrightMagenta";
        break;
    case ColorPalette16::BrightCyan:
        stream << "BrightCyan";
        break;
    case ColorPalette16::White:
        stream << "White";
        break;
    default:
        stream << "<invalid>";
    } 
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Color16& color)
{
    stream << color.color() << "(" << color.palette_index() << ")";
    return stream;
}
 
std::ostream& operator<<(std::ostream& stream, const Color256& color) {
    stream << color.palette_index();
    return stream; 
}
 
std::ostream& operator<<(std::ostream& stream, const Rgb& color) {
    stream << "(" << +color.red() << ", " << +color.green() << ", "
        << +color.blue() << ")";
    return stream;
}
 
std::ostream& operator<<(std::ostream& stream, const TerminalColor& color) {
    color.get_internal().print(stream);
    return stream;
}
 
std::ostream& operator<<(std::ostream& stream, const TerminalAttributes& attribs) {
    std::vector<std::string> string_frags;
    if(attribs.is_underlined()) {
        string_frags.push_back("underline");
    } 
    if(attribs.is_bold()) {
        string_frags.push_back("bold");
    }
    if(attribs.is_faint()) {
        string_frags.push_back("faint");
    }
    if(attribs.is_italic()) {
        string_frags.push_back("italic");
    }
    if(attribs.is_strikethrough()) {
        string_frags.push_back("strike");
    }
    if(attribs.is_negative()) {
        string_frags.push_back("negative");
    }
    if(attribs.is_hidden()) {
        string_frags.push_back("hidden");
    }
    if(attribs.blink() == BlinkMode::Slow) {
        string_frags.push_back("slow_blink");
    }
    if(attribs.blink() == BlinkMode::Fast) {
        string_frags.push_back("fast_blink");
    }
    int i = 0;
    for(auto& str : string_frags) {
        if(i != 0) {
            stream << ", ";
        }
        stream << str;
        i += 1;
    }
    if(string_frags.size() == 0) {
        stream << "normal";
    }

    return stream;
}
 
std::ostream& operator<<(std::ostream& stream, const TerminalFormatting& formatting) {
    stream << "[" << formatting.attributes() << "], fg=" << formatting.foreground_color()
        << ", bg=" << formatting.background_color();
    return stream; 
}
 
std::ostream& operator<<(std::ostream& stream, FontWeight weight) {
    switch(weight) {
    case FontWeight::Bold:
        stream << "Bold";
        break;
    case FontWeight::Faint:
        stream << "Faint";
        break;
    case FontWeight::Normal:
        stream << "Normal";
        break;
    default:
        stream << "<invalid>";
    }
    return stream;
}
 
std::ostream& operator<<(std::ostream& stream, BlinkMode mode) {
    switch(mode) {
    case BlinkMode::Off:
        stream << "Off";
        break;
    case BlinkMode::Slow:
        stream << "Slow";
        break;
    case BlinkMode::Fast:
        stream << "Fast";
        break;
    default:
        stream << "<invalid>";
    } 
    return stream;
}
 
}
