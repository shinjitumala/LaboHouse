/// @file http.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <labo/debug/Log.h>
#include <labo/server/http/Body.h>
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

Response::Response(Status status, path body_path, Headers headers)
  : status{ status }
  , headers{ headers }
  , body{ new Body{ body_path } } {};

Response::Response(Status status, Headers headers)
  : status{ status }
  , headers{ headers }
  , body{ nullptr }
{}

Response::~Response()
{
    delete body;
}

void
Response::print(ostream& os) const
{
    static const string http_protocol_ver{ "HTTP/1.1" };
    os << http_protocol_ver << " " << static_cast<uint>(status) << " "
       << to_string(status) << endl;
    for (auto& [data, value] : headers) {
        os << data << ": " << value << endl;
    }

    if (body) {
        // get length of file
        auto& ifs{ const_cast<ifstream&>(body->ifs) };
        ifs.seekg(0, ios::end);
        auto content_length{ ifs.tellg() };
        os << "Content-Length: " << content_length << endl << endl;

        // Reset ifs
        ifs.seekg(0);
        // copy entire stream into os
        copy(istreambuf_iterator<char>(ifs),
             istreambuf_iterator<char>(),
             ostreambuf_iterator<char>(os));
    }
    os << endl << endl;
}
};