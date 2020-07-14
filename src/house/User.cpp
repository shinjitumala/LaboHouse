/// @file User.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <nlohmann/json.hpp>
#include <labo/debug/Log.h>
#include <labo/house/User.h>

namespace labo {
User::User(const string id, const string cookie)
  : id{ id }
  , cookie{ cookie }
  , status{ Status::free }
{}

string
User::to_string(Status s)
{
    switch (s) {
        case Status::free:
            return "free";
        case Status::easy:
            return "easy";
        case Status::busy:
            return "busy";
        case Status::offline:
            return "offline";
        default:
            errs << "Invalid 'HIMADO': " << static_cast<uint>(s) << endl;
            failure();
    }
}

size_t
User::hash::operator()(const User& u) const
{
    return std::hash<string>()(u.cookie);
}

bool
User::operator==(const User& rhs) const
{
    return this == &rhs;
}

nlohmann::json User::to_json()const{
    nlohmann::json j;
    j["name"] = name;
    j["id"] = id;
    return j;
}
}