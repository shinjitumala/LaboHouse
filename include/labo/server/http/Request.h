/// @file http.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <labo/util/stream.h>
#include <map>
#include <string>

namespace labo::http {
using namespace std;

/// Represents a HTTP request
struct Request
{
    /// Request method.
    enum class Method
    {
        GET,
        POST,
    };

    /// Method of the request
    Method method;
    /// Path of the request
    string path;
    /// All header data
    map<string, string> headers;
    /// body
    string body;

    /// Set this from stream.
    /// @param is
    void deserialize(istream& is);
};
};