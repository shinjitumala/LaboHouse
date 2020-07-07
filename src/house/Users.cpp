/// @file Users.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.

#include <algorithm>
#include <cctype>
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
Users::add(string display_name, string student_id)
{
    static ulong name_counter{ 0 };
    lock_guard<mutex> lg{ mtx };
    if (auto itr{ usernames.find(display_name) }; itr != usernames.end()) {
        return *(itr->second);
    }
    auto user{ new User{ to_string(name_counter), display_name, student_id } };
    users.insert(user);
    usernames.insert({ display_name, user });
    cookies.insert({ to_string(name_counter), user });
    auto id{ name_counter };
    logs << "[Users] New user: { id = " << id << ", name = " << display_name
         << " }" << endl;
    name_counter++;
    return *user;
}

OptionalRef<User>
Users::get(string cookie) const
{
    if (auto itr{ cookies.find(cookie) }; itr == cookies.end()) {
        return OptionalRef<User>{};
    } else {
        return *itr->second;
    }
}

nlohmann::json
Users::to_json() const
{
    nlohmann::json j{ nlohmann::json::array() };
    for_each(usernames.begin(), usernames.end(), [&](auto& p) {
        j.push_back(p.first);
    });
    return j;
}

nlohmann::json
Users::to_json_sorted() const
{
    nlohmann::json j;
    unordered_map<User::Status, nlohmann::json> sorted;
    for (auto s{ User::Status::free };
         static_cast<char>(s) < static_cast<char>(User::Status::last);
         s = static_cast<User::Status>(static_cast<char>(s) + 1)) {
        sorted[s] = nlohmann::json::array();
    }
    for_each(users.begin(), users.end(), [&](auto u) {
        sorted[u->status].push_back(u->display_name);
    });
    for (auto [s, sj] : sorted) {
        j[User::to_string(s)] = sj;
    };

    return j;
}

};