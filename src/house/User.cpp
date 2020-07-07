/// @file User.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <labo/debug/Log.h>
#include <labo/house/User.h>

namespace labo {
User::User(const ulong id, const string display_name)
  : id{ id }
  , display_name{ display_name }
  , status_flag{ Status::free }
{}

User::Status
User::status() const
{
    return status_flag;
};

void
User::set_status(Status status)
{
    status_flag = status;
}

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
}