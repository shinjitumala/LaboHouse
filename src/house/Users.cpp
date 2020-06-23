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

namespace labo {
Users::~Users()
{
    for_each(users.begin(), users.end(), [](auto u) { delete u; });
}
ulong
Users::add_name(string name)
{
    static ulong name_counter{ 0 };
    lock_guard<mutex> lg{ mtx };
    if (usernames.count(name)) {
        return get_id(name);
    }
    auto user{ new User{ name_counter, name } };
    users.insert(user);
    usernames.insert({ name, user });
    ids.insert({ name_counter, user });
    auto id{ name_counter };
    logs << "[Users] New user: { id = " << id << ", name = " << name << " }"
         << endl;
    name_counter++;
    return id;
}

bool
Users::name_exists(string name) const
{
    return usernames.count(name);
}

ulong
Users::get_id(string name) const
{
    return usernames.at(name)->id;
}

string
Users::get_name(ulong id) const
{
    return ids.at(id)->display_name;
}

};