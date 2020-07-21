/// @file Quotes.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the SWORD tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <string>
namespace labo {
using namespace std;
struct Quotes
{
    /// Get a random quote.
    /// @return pair<string, string> Author, Quote
    static pair<string, string> get();
};
};