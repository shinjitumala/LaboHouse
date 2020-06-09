#pragma once

#include <iostream>
#include <streambuf>
#include <tuple>

namespace fpr {
using namespace std;
namespace log {
/// A virtual buffer used to insert a callback function with each '\n'
/// character.
/// @tparam Callback
template<class Callback>
class LineCallbackBuf : public streambuf
{
    using Base = streambuf;

  protected:
    /// The real buffer, we will send all the output here after checking them
    Base* const destination;
    /// The newline callback
    Callback& callback;
    /// Set to true when the next character is the first in a line.
    bool is_newline{ true };

  public:
    /// @param dest Final Destination of the output
    /// @param callback Use this to pass dynamically allocated CallBack
    /// functions.
    LineCallbackBuf(ostream& dest, Callback& callback)
      : destination{ dest.rdbuf() }
      , callback{ callback }
    {}

    /// Since we did not define a character, this function will be called for
    /// each character.
    /// @param c
    /// @return int
    int overflow(int c) override
    {
        if (c == traits_type::eof()) {
            return sync();
        } else if (is_newline) {
            is_newline = false;
            if (callback.call(*destination) == traits_type::eof()) {
                return traits_type::eof();
            }
            return destination->sputc(c);
        } else {
            if (c == '\n') {
                is_newline = true;
            }
            return destination->sputc(c);
        }
        return traits_type::eof();
    }
};

/// Helper class for 'LineCallbackBuf'.
/// Combines multiple callback classes to one.
/// @tparam Callbacks
template<class... Callbacks>
class CombinedCallback
{
    /// Tuple of callback classes.
    tuple<Callbacks&...> callbacks;

  public:
    /// Use this to initialize the callback classes with arguments.
    /// @param callbacks
    CombinedCallback(Callbacks&... callbacks)
      : callbacks(callbacks...)
    {}

    /// Called when LineCallbackBuf
    /// @param buf
    /// @return int
    int call(streambuf& buf)
    {
        int ret;
        // What have I done...?
        return apply(
          [&](auto&&... cb) {
              // This is done to silence the warning. Will probably be
              // optimized.
              if ((((void(ret = cb.call(buf)), true) &&
                    ret == streambuf::traits_type::eof()) ||
                   ...)) {
                  cerr << "Output error." << endl;
                  ::exit(1);
              };
              return ret;
          },
          callbacks);
    }
};
};
};