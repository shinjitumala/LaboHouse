/// @file http.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <labo/debug/Log.h>
#include <labo/server/http/Html.h>
#include <labo/server/http/Response.h>
#include <nlohmann/json.hpp>
#include <limits>
#include <regex>

namespace labo::http {
string
Response::to_string(Response::Status status)
{
    switch (status) {
        case Response::Status::OK:
            return "OK";
        case Response::Status::BAD_REQUEST:
            return "BAD REQUEST";
        case Response::Status::UNAURHORIZED:
            return "UNAUTHORIZED";
        case Response::Status::FORBIDDEN:
            return "FORBIDDEN";
        case Response::Status::NOT_FOUND:
            return "NOT FOUND";
    }
}

Response::Response(Status status, Headers headers)
  : status{ status }
  , headers{ headers }
  , body{ None{} }
{}

void
Response::print(ostream& os) const
{
    static const string http_protocol_ver{ "HTTP/1.0" };
    os << http_protocol_ver << " " << static_cast<uint>(status) << " "
       << to_string(status) << endl;
    for (auto& [data, value] : headers) {
        os << data << ": " << value << endl;
    }

    switch (body.index()) {
        case 0: { // None
            os << "Content-Length: 0" << endl;
        } break;
        case 1: { // Html
            // get length of file
            auto ifs{ get<Html>(body).read() };
            ifs.seekg(0, ios::end);
            auto content_length{ ifs.tellg() };
            os << "Content-Type: text/html; charset=UTF8" << endl;
            os << "Content-Length: " << content_length << endl << endl;
            /// Assume that the body is always an html for now.

            // Reset ifs
            ifs.seekg(0);
            // copy entire stream into os
            copy(istreambuf_iterator<char>(ifs),
                 istreambuf_iterator<char>(),
                 ostreambuf_iterator<char>(os));

        } break;
        case 2: { // Json
            auto s{ get<nlohmann::json>(body).dump() };
            os << "Content-Type: application/json; charset=UTF8" << endl;
            os << "Content-Length: " << s.size() << endl << endl;

            os << s;
        } break;
    }
    os << endl << endl;
}
};