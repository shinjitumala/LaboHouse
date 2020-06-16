/// @file http.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <labo/server/http/Body.h>
#include <labo/util/stream.h>
#include <map>
#include <string>

namespace labo::http {
using namespace std;

/// Represents a HTTP Response
class Response
{
  public:
    /// Status codes
    enum class Status : uint
    {
        OK = 200,
        NOT_FOUND = 404,
    };

  private:
    /// Alias for readability.
    using Headers = map<string, string>;

    /// Status code
    Status status;
    /// All header data
    Headers headers;
    /// The response body
    Body body;

  public:
    /// Construct a response.
    /// @param status The status.
    /// @param body The response body.
    /// @param headers Headers. Optional. NOTE: 'Content-Length' is not needed.
    Response(Status status, Body&& body, Headers headers = {});

    /// Print this Response to a stream.
    /// @param os
    void print(ostream& os) const;

  private:
    /// Helper funciton.
    static string to_string(Response::Status status);
};

};