/// @file User.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <mutex>
#include <nlohmann/json.hpp>
#include <unordered_set>

namespace labo {
using namespace std;
using ulong = unsigned long;
using Json = nlohmann::json;

/// Holds the data relevant to the user.
class User
{
  public:
    enum Status : unsigned char
    {
        sFree,    // Doing nothing
        sEasy,    // Working but available for some chit-chat
        sBusy,    // Do not disturb
        sOffline, // Offline. (For future use.)
        sLast,    // Final element
    };

    /// Convert status to string.
    /// @param s
    /// @return string
    static string to_string(Status s);

    /// Student id
    const string id;
    /// Identifier for the user. Also used as the cookie. (FIXME: Vulnerable)
    const string cookie;

    /// Display name
    string name;
    /// level for freeness (is that a word?)
    Status status;

    /// Receive notification when users here become online.
    unordered_set<const User*> watchlist;

  public:
    /// A user must have an id and a name.
    /// @param id
    /// @param cookie
    User(const string id, const string cookie);

    struct hash
    {
        /// Get hash.
        /// @param u
        /// @return size_t
        size_t operator()(const User& u) const;
    };

    /// @param rhs
    /// @return true If this is the same instance as rhs.
    /// @return false
    bool operator==(const User& rhs) const;

    /// Obtain this as json.
    /// @return Json
    Json to_json() const;

    /// Allow move.
    User(User&&) = default;

    bool in_watchlist(const User& u) const;
    void watchlist_add(const User& u);
    void watchlist_remove(const User& u);

  private:
    /// Disallow copy because users are supposed to be unique.
    User(const User&) = delete;
};
};