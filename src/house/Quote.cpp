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
        { "アインシュタイン",
          "私は天才ではありません。ただ、人より長く一つの事柄と付き合っていただ"
          "けです。" },
        { "ショーペンハウアー",
          "なんであれ、人は忘れることができる。ただ自分自身、己自身の存在を忘れ"
          "ることはできない。" },
        { "ヒポクラテス", "芸術は長く、人生は短し" },
        { "ウェルギリウス", "誰もがそれぞれの地獄を背負っている。" },
        { "フロイト",
          "自ら進んで求めた孤独や他者からの分離は、人間関係から生ずる苦悩に対し"
          "てもっとも手近な防衛となるものである。" },
        { "キケロ",
          "第一に必要なるものは大胆、第二に必要なるものも大胆、第三に必要なるも"
          "のも大胆なり。" },
    };
    static auto random_index = []() {
        using namespace chrono;
        static mt19937_64 mt{ static_cast<unsigned long>(
          high_resolution_clock::now().time_since_epoch().count()) };
        static uniform_int_distribution<size_t> dist(0, quotes.size() - 1);
        return dist(mt);
    };
    return quotes[random_index()];
};
};