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
#include <nlohmann/json.hpp>
#include <labo/util/rand.h>

namespace labo {
User&
Users::add(string id)
{
    unique_lock lg{ mtx };
    // Check if someone has the same id.
    if (auto itr{ ids.find(id) }; itr != ids.end()) {
        errs << "[Users] Same ID: " << id << endl;
        return *(itr->second);
    }
    auto cookie{ random_string(256) };
    for (; cookies.count(cookie); cookie = random_string(256)) {
    }
    auto [uitr, b]{ users.emplace(id, cookie) };
    auto& user{ const_cast<User&>(*uitr) };
    ids.insert({ id, &user });
    cookies.insert({ cookie, &user });
    logs << "[Users] New user: { id = " << id << " }" << endl;
    return user;
}

OptionalRef<User>
Users::by_id(string id)
{
    shared_lock lg{ mtx };
    if (auto itr{ ids.find(id) }; itr == ids.end()) {
        return OptionalRef<User>{};
    } else {
        return *itr->second;
    }
}

OptionalRef<User>
Users::by_cookie(string cookie)
{
    shared_lock lg{ mtx };
    if (auto itr{ cookies.find(cookie) }; itr == cookies.end()) {
        return OptionalRef<User>{};
    } else {
        return *itr->second;
    }
}

nlohmann::json
Users::to_json()
{
    shared_lock lg{ mtx };
    nlohmann::json j;
    unordered_map<User::Status, nlohmann::json> sorted;
    for (auto s{ User::Status::free };
         static_cast<char>(s) < static_cast<char>(User::Status::last);
         s = static_cast<User::Status>(static_cast<char>(s) + 1)) {
        sorted[s] = nlohmann::json::array();
    }
    for_each(users.begin(), users.end(), [&](auto& u) {
        sorted[u.status].push_back(u.name + "#" + u.id);
    });
    for (auto [s, sj] : sorted) {
        j[User::to_string(s)] = sj;
    };

    return j;
}
};