/// @file User.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <labo/debug/Log.h>
#include <labo/house/User.h>
#include <nlohmann/json.hpp>

namespace labo {
User::User(const string id, const string cookie)
  : id{ id }
  , cookie{ cookie }
  , status{ sFree }
{}

string
User::to_string(Status s)
{
    switch (s) {
        case sFree:
            return "Free";
        case sEasy:
            return "Easy";
        case sBusy:
            return "Busy";
        case sOffline:
            return "Offline";
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

nlohmann::json
User::to_json() const
{
    nlohmann::json j;
    j["name"] = name;
    j["id"] = id;
    return j;
}
}