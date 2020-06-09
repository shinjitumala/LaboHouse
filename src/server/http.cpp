/// @file http.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <experimental/source_location>
#include <labo/debug/Log.h>
#include <labo/server/http.h>
#include <limits>
#include <regex>

namespace labo::http {
string
to_string(Response::Status status)
{
    switch (status) {
        case Response::Status::OK:
            return "OK";
        case Response::Status::NOT_FOUND:
            return "NOT FOUND";
    }
}

void
Request::deserialize(istream& is)
{
    auto check_is = [&]() {
        if (is.bad()) {
            errs << "Parsing error." << endl;
            failure();
        }
    };

    logs << "Deserializing request..." << endl;
    string raw_method;
    is >> raw_method;
    check_is();
    if (raw_method == "GET") {
        method = Method::GET;
    } else if (raw_method == "POST") {
        method = Method::POST;
    } else {
        errs << "Unexpected Request: " << raw_method << endl;
        failure();
    }
    logs << raw_method << " ";

    is >> path;
    check_is();
    logs << path << " ";

    string protocol;
    is >> protocol;
    check_is();
    logs << protocol << endl;
    is.ignore(numeric_limits<streamsize>::max(), '\n');

    for (string line; getline(is, line);) {
        if (line.size() == 0 || line == "\r") {
            break;
        }
        const regex header_pattern{
            "^([^:]*):(.*)(\r)*"
        }; // HOLY SHIT FUCK YOU CARRIGE RETURN. FUCK YOU FUCK YOU FFUCK YOU.
        smatch matches;
        if (regex_match(line, matches, header_pattern) && matches.size() > 2) {
            headers.insert({ matches[1], matches[2] });
            logs << "[ " << matches[1] << ", " << matches[2] << " ]" << endl;
        } else {
            errs << "Failed to match header: " << line << endl;
            errs << matches.size() << endl;
        }
    }

    logs << "Done!" << endl;
}
};