/// @file Users.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.

#include <algorithm>
#include <labo/debug/Log.h>
#include <labo/house/User.h>
#include <labo/house/Users.h>
#include <labo/util/json.hpp>

namespace labo {
Users::~Users()
{
    for_each(users.begin(), users.end(), [](auto u) { delete u; });
}

User&
Users::add(string display_name)
{
    static ulong name_counter{ 0 };
    lock_guard<mutex> lg{ mtx };
    if (auto itr{ usernames.find(display_name) }; itr != usernames.end()) {
        return *(itr->second);
    }
    auto user{ new User{ name_counter, display_name } };
    users.insert(user);
    usernames.insert({ display_name, user });
    ids.insert({ name_counter, user });
    auto id{ name_counter };
    logs << "[Users] New user: { id = " << id << ", name = " << display_name
         << " }" << endl;
    name_counter++;
    return *user;
}

OptionalRef<User>
Users::get(string display_name) const
{
    if (auto itr{ usernames.find(display_name) }; itr == usernames.end()) {
        return OptionalRef<User>{};
    } else {
        return *itr->second;
    }
}

OptionalRef<User>
Users::get(ulong id) const
{
    if (auto itr{ ids.find(id) }; itr == ids.end()) {
        return OptionalRef<User>{};
    } else {
        return *itr->second;
    }
}

nlohmann::json
Users::to_json() const
{
    vector<string> names;
    names.reserve(users.size());
    for_each(usernames.begin(), usernames.end(), [&](auto& p) {
        names.push_back(p.first);
    });
    return names;
}

};