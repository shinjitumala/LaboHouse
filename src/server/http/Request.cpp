/// @file http.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <fstream>
#include <labo/debug/Log.h>
#include <labo/server/http/Request.h>
#include <labo/util/OptionalRef.h>
#include <limits>
#include <mutex>
#include <regex>
#include <sstream>

namespace labo::http {
ofstream logs{ "http1_requests.log" };
mutex mtx;

istream&
get_line(istream& is, string& str)
{
    str.clear();
    istream::sentry s{ is, true };
    auto buf{ is.rdbuf() };
    while (true) {
        auto c{ buf->sbumpc() };
        switch (c) {
            case '\r':
                if (buf->sgetc() == '\n') {
                    buf->sbumpc();
                } else {
                    is.setstate(ios::eofbit);
                    errs << "Unexpected '\\r': " << str << endl;
                }
                return is;
            case std::streambuf::traits_type::eof():
                is.setstate(std::ios::eofbit);
                errs << "Line has no end: " << str << endl;
                return is;
            default:
                str += static_cast<char>(c);
        }
    }
}

void
Request::deserialize(istream& is)
{
    lock_guard lg{ mtx };
    auto is_valid = [&]() {
        if (is.bad() || is.eof()) {
            errs << "[Request] error." << endl;
            return false;
        }
        return true;
    };

    logs << "[Request] Parsing..." << endl;
    valid = false;

    /// Get first line
    for (string line; get_line(is, line);) {
        if (!line.size()) {
            continue;
        }
        istringstream is{ line };
        string raw_method_tag;
        is >> raw_method_tag;
        if (raw_method_tag == "GET") {
            method_tag = Method::GET;
        } else if (raw_method_tag == "POST") {
            method_tag = Method::POST;
        } else {
            errs << "Unexpected Request: " << raw_method_tag << endl;
            return;
        }
        logs << "Method: " << raw_method_tag << endl;

        is >> raw_path;
        static const regex query_pattern{ "^(/[^\?]*)\\?\?(.*)$" };
        if (smatch matches; regex_match(raw_path, matches, query_pattern)) {
            raw_path = matches[1];
            string values{ matches[2] };
            static const regex value_pattern{ "([^&]+)=([^&]+)" };
            for (sregex_iterator itr{
                   values.begin(), values.end(), value_pattern };
                 itr != sregex_iterator{};
                 itr++) {
                // Must parse values if needed.
                auto& matches{ *itr };
                query.insert({ matches[1], matches[2] });
            }
        } else {
            errs << "Error parsing URI: " << raw_path;
            failure();
        }

        logs << "Path: " << raw_path << endl;
        if (query.size()) {
            logs << "Values: {" << endl;
            for (auto [name, value] : query) {
                logs << "  " << name << ": " << value << "," << endl;
            }
            logs << "}" << endl;
        }

        string protocol;
        is >> protocol;
        logs << "Protocol Version: " << protocol << endl;
        break;
    }

    valid = is_valid();
    if (!valid) {
        return;
    }

    /// Get rest.
    logs << "Headers {" << endl;
    for (string line; get_line(is, line);) {
        if (line.size() == 0) {
            break;
        }

        const regex header_pattern{
            "^([^:]+): (.+)$"
        }; // HOLY SHIT FUCK YOU CARRIGE RETURN. FUCK YOU FUCK YOU FFUCK YOU.
        if (smatch matches;
            regex_match(line, matches, header_pattern) && matches.size() > 2) {
            headers.insert({ matches[1], matches[2] });
            logs << "  { " << matches[1] << ", " << matches[2] << " }, "
                 << endl;
        } else {
            errs << "Failed to match header: " << line << endl;
        }
    }
    logs << "}" << endl;
    valid = is_valid();
    if (!valid) {
        return;
    }

    valid = true;
    logs << "[Request] Done parsing!" << endl;
}

Request::Method
Request::method() const
{
    return method_tag;
}

const string
Request::path() const
{
    return raw_path;
}

OptionalRef<const string>
Request::query_value(const string value_name) const
{
    auto itr{ query.find(value_name) };
    if (itr == query.end()) {
        errs << "[Request] No query value: " << value_name << endl;
        return OptionalRef<const string>{};
    }

    return itr->second;
}

OptionalRef<const string>
Request::header_value(const string value_name) const
{
    auto itr{ headers.find(value_name) };
    if (itr == headers.end()) {
        errs << "[Request] No header value: " << value_name << endl;
        return OptionalRef<const string>{};
    }

    return itr->second;
}
};