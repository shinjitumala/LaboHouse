/// @file rand.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the SWORD tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <functional>
#include <labo/util/rand.h>
#include <random>
namespace labo {
mt19937 rand(::time(0));
auto char_rand = bind(uniform_int_distribution<char>('a', 'z'), rand);

string
random_string(size_t len)
{
    string s;
    s.resize(len);
    for (auto& c : s) {
        c = char_rand();
    }
    return s;
}
};
