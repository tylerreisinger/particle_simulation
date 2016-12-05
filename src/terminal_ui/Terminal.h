#ifndef PS_TERMINAL_H_
#define PS_TERMINAL_H_

#include <iostream>
#include <memory>
#include <cassert>

#include <boost/optional.hpp>

namespace term {

class Terminal;
class TerminalColor;
class Color16;

enum class ColorFormat {
    Palette16,
    Palette256,
    Rgb
};

enum class BlinkMode {
    Off,
    Slow,
    Fast
};

enum class FontWeight {
    Normal,
    Bold,
    Faint
};

enum class ColorPalette16 {
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    DullWhite,
    Gray,
    BrightRed,
    BrightGreen,
    BrightYellow,
    BrightBlue,
    BrightMagenta,
    BrightCyan,
    White 
};

class Rgb {
public:
    constexpr Rgb() = default;
    constexpr Rgb(uint8_t red, uint8_t green, uint8_t blue):
        m_red(red), m_green(green), m_blue(blue) {}
    ~Rgb() = default;

    constexpr Rgb(const Rgb& other) = default;
    constexpr Rgb(Rgb&& other) noexcept = default;
    constexpr Rgb& operator =(const Rgb& other) = default;
    constexpr Rgb& operator =(Rgb&& other) noexcept = default;

    constexpr bool operator ==(const Rgb& rhs) const {
        return (m_red == rhs.m_red && m_green == rhs.m_green && m_blue == rhs.m_blue);
    }
    constexpr bool operator !=(const Rgb& rhs) const {
        return !(*this == rhs);
    }

    constexpr uint8_t red() const {return m_red;}
    constexpr uint8_t green() const {return m_green;}
    constexpr uint8_t blue() const {return m_blue;}
    void set_red(uint8_t red) {m_red = red;}
    void set_green(uint8_t green) {m_green = green;}
    void set_blue(uint8_t blue) {m_blue = blue;}

    void apply_foreground(Terminal& terminal) const;
    void apply_background(Terminal& terminal) const;

    static constexpr ColorFormat format() {
        return ColorFormat::Rgb;
    }

private:
    uint8_t m_red = 0;
    uint8_t m_green = 0;
    uint8_t m_blue = 0;
};

class Color256 {
public:
    constexpr explicit Color256(unsigned int palette_index):
        m_index(palette_index) {
    }

    ~Color256() = default;

    constexpr Color256(const Color256& other) = default;
    constexpr Color256(Color256&& other) noexcept = default;
    constexpr Color256& operator =(const Color256& other) = default;
    constexpr Color256& operator =(Color256&& other) noexcept = default;

    constexpr bool operator ==(const Color256& rhs) const {
        return m_index == rhs.m_index;
    }
    constexpr bool operator !=(const Color256& rhs) const {
        return !(*this == rhs);
    }

    void apply_foreground(Terminal& terminal) const;
    void apply_background(Terminal& terminal) const;

    constexpr bool is_standard_color() const {return m_index <= 0x0F;}
    constexpr bool is_extended_color() const {return (m_index > 0x0F && m_index <= 0xE7);}
    constexpr bool is_grayscale() const {return (m_index > 0xE7);}

    constexpr unsigned int palette_index() const {return m_index;}
    void set_palette_index(unsigned int index) {
        assert(index < 255);
        assert(index >= 0);
    }

    static constexpr ColorFormat format() {
        return ColorFormat::Palette256;
    }
    
    static constexpr Color256 make_grayscale(unsigned int index) {
        assert(index < 24);
        return Color256(index + 0xE8);
    }

private: 
    unsigned int m_index;
};

class Color16 {
public:
    constexpr Color16() = default;
    constexpr Color16(ColorPalette16 color):
        m_color_index(static_cast<unsigned int>(color)) {}
    ~Color16() = default;

    constexpr Color16(const Color16& other) = default;
    constexpr Color16(Color16&& other) noexcept = default;
    constexpr Color16& operator =(const Color16& other) = default;
    constexpr Color16& operator =(Color16&& other) noexcept = default;

    constexpr bool operator ==(const Color16& rhs) const {
        return (rhs.m_color_index == rhs.m_color_index);
    }
    constexpr bool operator !=(const Color16& rhs) const {
        return !(*this == rhs);
    }

    constexpr unsigned int palette_index() const {return m_color_index;}
    constexpr ColorPalette16 color() const {return static_cast<ColorPalette16>(m_color_index);}
    void set_palette_index(unsigned int idx) {
        assert(idx < 16);
        assert(idx >= 0);
        m_color_index = idx;
    } 
    void set_color(ColorPalette16 color) {m_color_index = static_cast<unsigned int>(m_color_index);}

    void apply_foreground(Terminal& terminal) const;
    void apply_background(Terminal& terminal) const;

    static constexpr ColorFormat format() {
        return ColorFormat::Palette16;
    }

    static Color16 from_palette_index(unsigned int index) {
        assert(index < 16);
        assert(index >= 0);
        return Color16(index);
    }

private:
    explicit Color16(unsigned int palette_index):
        m_color_index(palette_index) {}

    unsigned int m_color_index = 0;
};

namespace {

class TerminalColorBase {
public:
    TerminalColorBase() = default;
    virtual ~TerminalColorBase() = default;

    virtual void apply_foreground(Terminal& terminal) const = 0;
    virtual void apply_background(Terminal& terminal) const = 0;

    virtual std::unique_ptr<TerminalColorBase> clone() const = 0;

    virtual ColorFormat format() const = 0;
    virtual bool is_equal(const TerminalColorBase& rhs) const = 0;

    virtual void print(std::ostream& stream) const = 0;
};

template<typename ColorType>
class PolymorphicTerminalColor: public TerminalColorBase {
public:
    PolymorphicTerminalColor() = default;
    PolymorphicTerminalColor(const ColorType& color):
        m_color(color) {}

    virtual ~PolymorphicTerminalColor() = default;

    virtual void apply_foreground(Terminal& terminal) const override {
        m_color.apply_foreground(terminal);
    }
    virtual void apply_background(Terminal& terminal) const override {
        m_color.apply_background(terminal);
    }

    virtual std::unique_ptr<TerminalColorBase> clone() const override {
        return std::unique_ptr<TerminalColorBase>(
            new PolymorphicTerminalColor<ColorType>(m_color));
    }

    virtual ColorFormat format() const override {
        return m_color.format();
    }

    virtual void print(std::ostream& stream) const override {
        stream << m_color;
    }

    virtual bool is_equal(const TerminalColorBase& rhs) const override {
        if(format() == rhs.format()) {
            return (m_color == static_cast<const PolymorphicTerminalColor<ColorType>&>
                (rhs).m_color);
        }
        return false;
    }

    ColorType& get_color() {
        return m_color;
    }

    const ColorType& get_color() const {
        return m_color;
    }

private:
    ColorType m_color;
};

    template<typename ColorFormat>
    boost::optional<ColorFormat> extract_helper(const TerminalColor& color);
}

class TerminalColor {
public:
    TerminalColor():
        m_color(std::make_unique<PolymorphicTerminalColor<Color16>>(
                ColorPalette16::DullWhite)) {
    }
    TerminalColor(std::unique_ptr<TerminalColorBase> color):
        m_color(std::move(color)) {
    }
    TerminalColor(const Color16& color):
        m_color(std::make_unique<PolymorphicTerminalColor<Color16>>(color)) {}
    TerminalColor(const Color256& color):
        m_color(std::make_unique<PolymorphicTerminalColor<Color256>>(color)) {}
    TerminalColor(const Rgb& color):
        m_color(std::make_unique<PolymorphicTerminalColor<Rgb>>(color)) {}
    
    ~TerminalColor() = default;

    TerminalColor(const TerminalColor& other):
        m_color(other.m_color->clone()) {}

    TerminalColor(TerminalColor&& other) noexcept = default;
    TerminalColor& operator =(const TerminalColor& other){
        m_color = other.m_color->clone();
        return *this;
    }

    TerminalColor& operator =(TerminalColor&& other) noexcept = default;

    bool operator ==(const TerminalColor& rhs) const {
        return m_color->is_equal(*rhs.m_color);
    }

    bool operator !=(const TerminalColor& rhs) const {
        return !(*this == rhs);
    }

    void apply_foreground(Terminal& terminal) const {
        m_color->apply_foreground(terminal);
    }
    void apply_background(Terminal& terminal) const {
        m_color->apply_background(terminal);
    }

    ColorFormat format() const {
        return m_color->format();
    }

    TerminalColor clone() const {
        return TerminalColor(m_color->clone());
    }

    TerminalColorBase& get_internal() {
        return *m_color;
    }
    const TerminalColorBase& get_internal() const {
        return *m_color;
    }

    template <typename ColorFormat>
    boost::optional<ColorFormat> extract() const {
        return extract_helper<ColorFormat>(*this);
    }

private:
    std::unique_ptr<TerminalColorBase> m_color;
};

namespace {

    template<>
    boost::optional<Color16> extract_helper<Color16>(const TerminalColor& color) {
        if(color.format() == ColorFormat::Palette16) {
            return 
                static_cast<const PolymorphicTerminalColor<Color16>&>
                    (color.get_internal()).get_color();
        }
        return boost::none;
    }

    template<>
    boost::optional<Color256> extract_helper<Color256>(const TerminalColor& color) {
        if(color.format() == ColorFormat::Palette256) {
            return 
                static_cast<const PolymorphicTerminalColor<Color256>&>
                    (color.get_internal()).get_color();
        }
        return boost::none;
    }

    template<>
    boost::optional<Rgb> extract_helper<Rgb>(const TerminalColor& color) {
        if(color.format() == ColorFormat::Rgb) {
            return 
                static_cast<const PolymorphicTerminalColor<Rgb>&>
                    (color.get_internal()).get_color();
        }
        return boost::none;
    }
    
}

template <typename ColorType>
inline TerminalColor make_terminal_color(const ColorType& color) {
    return TerminalColor(
        std::make_unique<PolymorphicTerminalColor<ColorType>>(color));
}

class TerminalAttributes {
public:
    TerminalAttributes() = default;
    ~TerminalAttributes() = default;

    TerminalAttributes(const TerminalAttributes& other) = default;
    TerminalAttributes(TerminalAttributes&& other) noexcept = default;
    TerminalAttributes& operator =(const TerminalAttributes& other) = default;
    TerminalAttributes& operator =(TerminalAttributes&& other) noexcept = default;

    bool is_underlined() const {return m_underline;}
    bool is_italic() const {return m_italic;}
    bool is_strikethrough() const {return m_strikethrough;}
    bool is_negative() const {return m_negative;}
    bool is_hidden() const {return m_hidden;}
    BlinkMode blink() const {return m_blink;}
    FontWeight weight() const {return m_weight;}
    bool is_bold() const {return m_weight == FontWeight::Bold;}
    bool is_faint() const {return m_weight == FontWeight::Faint;}

    TerminalAttributes& set_underlined(bool enabled) {m_underline = enabled; 
        return *this;}
    TerminalAttributes& set_italic(bool enabled) {m_italic = enabled; 
        return *this;}
    TerminalAttributes& set_strikethrough(bool enabled) {m_strikethrough = enabled; 
        return *this;}
    TerminalAttributes& set_negative(bool enabled) {m_negative = enabled; return *this;}
    TerminalAttributes& set_hidden(bool enabled) {m_hidden = enabled; return *this;}
    TerminalAttributes& set_blink(BlinkMode mode) {m_blink = mode; return *this;}
    TerminalAttributes& set_weight(FontWeight weight) {m_weight = weight; return *this;}
    TerminalAttributes& set_bold(bool enabled) {
        m_weight = (enabled ? FontWeight::Bold : FontWeight::Normal); 
        return *this;
    }
    TerminalAttributes& set_faint(bool enabled) {
        m_weight =
        (enabled ? FontWeight::Faint : FontWeight::Normal); 
        return *this;
    }

private:
    BlinkMode m_blink = BlinkMode::Off;
    FontWeight m_weight = FontWeight::Normal;
    bool m_underline = false;
    bool m_italic = false;
    bool m_strikethrough = false;
    bool m_negative = false;
    bool m_hidden = false;
};

class TerminalFormatting {
public:
    TerminalFormatting() = default;
    TerminalFormatting(const TerminalAttributes& attributes,
        TerminalColor foreground_color, TerminalColor background_color):
            m_attributes(attributes), m_foreground_color(std::move(foreground_color)),
            m_background_color(std::move(background_color)) {
    }

    ~TerminalFormatting() = default;

    TerminalFormatting(const TerminalFormatting& other) = default;
    TerminalFormatting(TerminalFormatting&& other) noexcept = default;
    TerminalFormatting& operator =(const TerminalFormatting& other) = default;
    TerminalFormatting& operator =(TerminalFormatting&& other) noexcept = default;

    TerminalAttributes& attributes() {
        return m_attributes;
    }
    const TerminalAttributes& attributes() const {
        return m_attributes;
    }
    TerminalColor& foreground_color() {
        return m_foreground_color;
    }
    const TerminalColor& foreground_color() const {
        return m_foreground_color;
    }
    TerminalColor& background_color() {
        return m_background_color;
    }
    const TerminalColor& background_color() const {
        return m_background_color;
    }
    void set_attributes(const TerminalAttributes& attributes) {
        m_attributes = attributes;
    }
    void set_foreground_color(TerminalColor color) {
        m_foreground_color = std::move(color);
    }
    void set_background_color(TerminalColor color) {
        m_background_color = std::move(color);
    }

private: 
    TerminalAttributes m_attributes;
    TerminalColor m_foreground_color;
    TerminalColor m_background_color;
};

class Terminal {
public:
    constexpr static const char* CSI = "\033[";

    ~Terminal() = default;

    Terminal(const Terminal& other) = delete;
    Terminal(Terminal&& other) noexcept = default;
    Terminal& operator =(const Terminal& other) = delete;
    Terminal& operator =(Terminal&& other) noexcept = default;

    void clear();

    void set_underline(bool enable);
    void set_bold(bool enable);
    void set_faint(bool enable);
    void set_font_weight(FontWeight mode);
    void set_italic(bool enable);
    void set_strikethrough(bool enable);
    void set_blink(BlinkMode mode);
    void set_negative(bool enable);
    void set_hidden(bool enable);
    void reset_formatting();

    void set_foreground_color(const Color16& color);
    void set_background_color(const Color16& color);
    void set_foreground_color(const Color256& color);
    void set_background_color(const Color256& color);
    void set_foreground_color(const Rgb& color);
    void set_background_color(const Rgb& color);
    void set_foreground_color(const TerminalColor& color);
    void set_background_color(const TerminalColor& color);

    void set_attributes(const TerminalAttributes& attributes);
    void set_formatting(const TerminalFormatting& formatting);

    const TerminalFormatting& formatting() const {
        return m_format;
    }
    const TerminalAttributes& attributes() const {
        return m_format.attributes();
    }

    static Terminal& instance() {
        if(m_instance == nullptr) {
            m_instance = std::unique_ptr<Terminal>(new Terminal());
        }
        return *m_instance;
    }

private:
    Terminal() = default;

    TerminalFormatting m_format;
    static std::unique_ptr<Terminal> m_instance;
};

std::ostream& operator <<(std::ostream& stream, ColorPalette16 color);
std::ostream& operator <<(std::ostream& stream, FontWeight weight);
std::ostream& operator <<(std::ostream& stream, BlinkMode mode);
std::ostream& operator <<(std::ostream& stream, const Color16& color);
std::ostream& operator <<(std::ostream& stream, const Color256& color);
std::ostream& operator <<(std::ostream& stream, const Rgb& color);
std::ostream& operator <<(std::ostream& stream, const TerminalColor& color);
std::ostream& operator <<(std::ostream& stream, const TerminalAttributes& attribs);
std::ostream& operator <<(std::ostream& stream, const TerminalFormatting& formatting);

}

#endif
