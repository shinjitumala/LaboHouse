/// @file http.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <experimental/source_location>
#include <labo/debug/Log.h>
#include <labo/server/http/Response.h>
#include <limits>
#include <regex>

namespace labo::http {
string
Response::to_string(Response::Status status)
{
    switch (status) {
        case Response::Status::OK:
            return "OK";
        case Response::Status::NOT_FOUND:
            return "NOT FOUND";
    }
}

void
Response::print(ostream& os) const
{}
};