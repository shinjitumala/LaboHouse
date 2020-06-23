/// @file Users.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once

#include <mutex>
#include <unordered_map>
#include <unordered_set>

namespace labo {
using namespace std;
class User;

/// Dictionary of users.
class Users
{
    /// Mutex used to avoid data corruption.
    mutex mtx;
    /// All users. Owned by this.
    unordered_set<User*> users;
    /// Used for name lookups.
    unordered_map<string, User*> usernames;
    /// Used for id lookups.
    unordered_map<ulong, User*> ids;

  public:
    /// We own users.
    ~Users();

    /// Adds a new name
    /// @param name
    /// @return ulong the cookie for the newly added name
    ulong add_name(string name);

    /// Check if a name exists.
    /// @param name
    /// @return true If 'name' is taken.
    /// @return false
    bool name_exists(string name) const;

    /// Fatal error if missing.
    /// @param name
    /// @return ulong
    ulong get_id(string name) const;
    /// Fatal error if missing.
    /// @param id
    /// @return string
    string get_name(ulong id) const;
};
};