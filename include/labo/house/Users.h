/// @file Users.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once

#include <labo/util/OptionalRef.h>
#include <labo/util/json.hpp>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

namespace labo {
using namespace std;
using ulong = unsigned long;
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

    /// Adds a new user
    /// Note that it will return the existing user if there is one.
    /// @param display_name
    /// @return OptionalRef<User> The new user.
    User& add(string display_name);

    /// Fatal error if missing.
    /// @param display_name
    /// @return OptionalRef<User>
    OptionalRef<User> get(string display_name) const;
    /// Fatal error if missing.
    /// @param id
    /// @return OptionalRef<User>
    OptionalRef<User> get(ulong id) const;

    /// Convert Users into a json array.
    /// @return nlohmann::json
    nlohmann::json to_json() const;
};
};