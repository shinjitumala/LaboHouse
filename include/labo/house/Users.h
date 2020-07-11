/// @file Users.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once

#include <labo/house/User.h>
#include <labo/util/OptionalRef.h>
#include <labo/util/json.hpp>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

namespace labo {
using namespace std;
using ulong = unsigned long;

/// Dictionary of users.
class Users
{
    /// Mutex used to avoid data corruption.
    mutex mtx;
    /// All users. Owned by this.
    unordered_set<User> users;
    /// Used for id lookups.
    unordered_map<string, User*> ids;
    /// Used for cookie lookups.
    unordered_map<string, User*> cookies;

  public:
    /// Adds a new user. Attempting to add a user with the same ID is a fatal
    /// error. Note that it will return the existing user if there is one.
    /// @param id
    /// @return The new user.
    User& add(string id);

    /// @param id
    /// @return OptionalRef<User>
    OptionalRef<User> by_id(string id) const;
    /// @param cookie
    /// @return OptionalRef<User>
    OptionalRef<User> by_cookie(string cookie) const;

    /// Convert Users into a json array.
    /// @return nlohmann::json
    nlohmann::json to_json() const;

  private:
    /// Must be unique.
    Users(const Users&) = delete;
};
};