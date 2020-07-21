/// @file Quote.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the SWORD tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <chrono>
#include <iostream>
#include <labo/house/Quotes.h>
#include <random>
#include <string_view>
#include <vector>

namespace labo {

pair<string, string>
Quotes::get()
{
    static const vector<pair<string, string>> quotes{
        { "Socrates",
          "One thing i know,that i know nothing.This is the source of my "
          "wisdom." },
        { "Plato",
          "Good people do not need law to tell them to act responsibly, while "
          "bad people will find a way around the laws." },
        { "Aristotle",
          "Anybody can become angry - that is easy, but to be angry with the "
          "right person and to the right degree and at the right time and for "
          "the right purpose, and in the right way - that is not within "
          "everybody's power and is not easy." },
        { "Epicurus",
          "You don't develop courage by being happy in your relationships "
          "everyday. You develop it by surviving difficult times and "
          "challenging adversity." },
        { "Heraclitus", "There is nothing permanent except change." },
        { "Socrates",
          "By all means, get married: if you find a good wife, you'll be "
          "happy; if not, you'll become a philosopher." },
        { "Alexander the Great",
          "I am indebted to my father for living, but to my teacher for living "
          "well." },
        { "Plutarch",
          "The real destroyer of the liberties of the people is he who spreads "
          "among them bounties, donations and benefits." },
        { "Democritus",
          "Nothing exists except atoms and empty space; everything else is "
          "just opinion." },
        { "Aristotle",
          "Love is composed of a single soul inhabiting two bodies." },
        { "Socrates",
          "All men's souls are immortal, but the souls of the righteous are "
          "immortal and divine." },
        { "Plato",
          "He who steals a little steals with the same wish as he who steals "
          "much, but with less power." },
        { "FPR",
          "Don't keep incompetent people at the top. It's for everyone's "
          "good." },
    };
    static auto random_index = []() {
        using namespace chrono;
        static mt19937_64 mt{ static_cast<unsigned long>(
          high_resolution_clock::now().time_since_epoch().count()) };
        static uniform_int_distribution<size_t> dist(0, quotes.size());
        return dist(mt);
    };
    return quotes[random_index()];
};
};