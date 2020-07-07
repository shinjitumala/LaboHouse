/// @file User.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <mutex>

namespace labo {
using namespace std;
using ulong = unsigned long;

/// Holds the data relevant to the user.
struct User
{
  public:
    enum class Status : unsigned char
    {
        free, // Doing nothing
        easy, // Working but available for some chit-chat
        busy, // Do not disturb
        offline,
        last, // Final element
    };
    /// Convert status to string.
    /// @param s
    /// @return string
    static string to_string(Status s);

    /// Identifier for the user. Also used as the cookie. (FIXME: Vulnerable)
    string cookie;
    /// Unique ID.
    string id;

    /// Display name
    string display_name;

    /// level for freeness (is that a word?)
    Status status;

    /// Allow move.
    User(User&&) = default;
    /// Disallow copy because users are supposed to be unique.
    User(const User&) = delete;
};
};