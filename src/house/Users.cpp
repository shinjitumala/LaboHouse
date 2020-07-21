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
#include <labo/util/rand.h>
#include <nlohmann/json.hpp>

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
    users.emplace_back(id, cookie);
    auto& user{ const_cast<User&>(users.back()) };
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

Json
Users::to_json()
{
    shared_lock lg{ mtx };
    Json j;
    for_each(users.begin(), users.end(), [&](auto& u) {
        j[User::to_string(u.status)].push_back(
          Json{ { "name", u.name }, { "id", u.id } });
    });
    return j;
}
};