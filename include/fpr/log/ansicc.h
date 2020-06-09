#pragma once

#include <iostream>
#include <ostream>

namespace fpr {
using namespace std;
namespace log {
/// Used to print ANSI escape code to decorate terminal output.
namespace ansi {
/// Foreground Color code. Add 10 to convert to backgroudn color code.
enum class Color
{
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37,
    NONE,
};

/// Effect (Bold, Underline, etc...)
enum class Effect
{
    RESET = 0,
    BOLD = 1,
    FAINT = 2,
    ITALIC = 3,
    UNDERLINE = 4,
    SLOW_BLINK = 5,
    FAST_BLINK = 6,
    INVERSE = 7,
    CONCEAL = 8,
    CROSS_OUT = 9,
    BOLD_OFF = 21,
    UNDERLINE_OFF = 24,
    INVERSE_OFF = 27,
    FRAMED = 51,
    NONE,
};

/// Use this to change the color and effects of the terminal output.
/// @tparam fg Foreground color.
/// @tparam bg Background color.
/// @tparam e Effect type.
template<Color fg, Color bg, Effect e>
struct Colorizer
{
    /// Print the ansi color code to change terminal output.
    /// @param os
    /// @return ostream&
    inline ostream& print(ostream& os) const
    {
        os << "\x1b["; // Escape Character
        if constexpr (fg != Color::NONE) {
            os << static_cast<uint>(fg);
        }
        if constexpr (bg != Color::NONE) {
            if constexpr (fg != Color::NONE) {
                os << ";";
            }
            os << static_cast<uint>(bg) + 10;
        }
        if constexpr (e != Effect::NONE) {
            if constexpr (fg != Color::NONE || fg != Color::NONE) {
                os << ";";
            }
            os << static_cast<uint>(e);
        }
        os << "m"; // End
        return os;
    }
};

/// Use this to reset the previously set ansi code state.
struct Reset
{
    /// Prints the code to reset ansi code state.
    /// @param os
    /// @return ostream&
    static inline ostream& print(ostream& os)
    {
        os << "\x1b[0m"; // Reset
        return os;
    }
};
};
};
};