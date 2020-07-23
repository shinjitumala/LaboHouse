/// @file stream.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the SWORD tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <istream>
#include <ostream>
#include <sstream>

namespace labo {
using namespace std;

/// Black magic
/// @tparam T
template<class T>
class is_printable
{
    template<class U>
    static constexpr auto test(int)
      -> decltype(declval<U>().print(declval<ostream&>()), true_type());
    template<class>
    static constexpr auto test(...) -> false_type;

  public:
    static constexpr bool value{ is_same_v<decltype(test<T>(0)), true_type> };
};

/// I thought I heard someone say, "I can read C++"
/// @tparam O
/// @tparam true
/// @param os
/// @param o
/// @return ostream&
template<class O, enable_if_t<is_printable<O>::value, bool> = true>
ostream&
operator<<(ostream& os, const O& o)
{
    o.print(os);
    return os;
}

/// MORE black magic. No, just kidding its the same shit.
/// @tparam T
template<class T>
class is_deserializable
{
    template<class U>
    static constexpr auto test(int)
      -> decltype(declval<U>().deserialize(declval<istream&>()), true_type());
    template<class>
    static constexpr auto test(...) -> false_type;

  public:
    static constexpr bool value{ is_same_v<decltype(test<T>(0)), true_type> };
};

/// Muahahahahahahaha
/// @tparam I
/// @tparam true
/// @param is
/// @param i
/// @return istream&
template<class I, enable_if_t<is_deserializable<I>::value, bool> = true>
istream&
operator>>(istream& is, I& i)
{
    i.deserialize(is);
    return is;
}

template<class O, enable_if_t<is_printable<O>::value, bool> = true>
string
to_string(O&& o)
{
    ostringstream oss;
    oss << o;
    return oss.str();
}
}