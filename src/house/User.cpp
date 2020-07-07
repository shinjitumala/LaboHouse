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