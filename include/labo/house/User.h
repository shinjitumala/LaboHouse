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
class User
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
    const ulong id;
    /// Display name
    const string display_name;

  private:
    /// level for freeness (is that a word?)
    Status status_flag;

  public:
    /// A user must have an id and a name.
    /// @param id
    /// @param display_name
    User(const ulong id, const string display_name);

    /// @return Status The current status of the user.
    Status status() const;

    /// Change status of user.
    /// @param status
    void set_status(Status status);

  private:
    /// Disallow copy because users are supposed to be unique.
    User(const User&) = delete;
};
};