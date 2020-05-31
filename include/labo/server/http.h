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

/// Represents a HTTP Response
struct Response
{
    /// Status codes
    enum class Status
    {
        OK = 200,
        NOT_FOUND = 404,
    };

    /// Status code
    Status status;
    /// All header data
    map<string, string> headers;
    /// body
    string body;

    /// Print this Response to a stream.
    /// @param os
    void print(ostream& os) const;

  private:
    /// Helper funciton.
    string to_string(Response::Status status);
};

};