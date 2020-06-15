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

/// Represents a HTTP Response
class Response
{
    /// Status codes
    enum class Status
    {
        OK = 200,
        NOT_FOUND = 404,
    };

    /// Alias for readability.
    using Headers = map<string, string>;

    /// Status code
    Status status;
    /// All header data
    Headers headers;

  public:
    Response(Status status, Headers headers = {}, Body body);

    /// Print this Response to a stream.
    /// @param os
    void print(ostream& os) const;

  private:
    /// Helper funciton.
    string to_string(Response::Status status);
};

};