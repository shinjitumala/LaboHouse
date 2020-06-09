#pragma once

#include <ostream>

namespace fpr {
using namespace std;
namespace log {
/// Use this on 'BufNewLine' to insert indent at every line
class Indent
{
    /// String for one indent.
    const string one_indent{ "    " };
    /// String for the current indent.
    string indent;

  public:
    /// Increase indent by one.
    inline void inc() { indent += one_indent; }
    /// Decrease indent by one.
    inline void dec()
    {
        indent.erase(indent.end() - one_indent.length(), indent.end());
    }

    /// This is called to insert indent at every line.
    /// @param buf
    /// @return int
    int call(streambuf& buf)
    {
        return buf.sputn(indent.c_str(), indent.size());
    }
};
};
};