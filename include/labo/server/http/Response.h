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
        BAD_REQUEST = 400,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
    };

  private:
    /// Alias for readability.
    using Headers = map<string, string>;
#if __has_include(<filesystem>)
    /// Alias for convenience.
    using path = filesystem::path;
#else
    /// Alias for convenience.
    using path = experimental::filesystem::path;
#endif

    /// Status code
    Status status;
    /// All header data
    Headers headers;
    /// The response body
    Body* const body;

  public:
    /// Construct a response.
    /// @param status The status.
    /// @param body_path Path to the html content.
    /// @param headers Headers. Optional. NOTE: 'Content-Length' is not needed.
    Response(Status status, path body_path, Headers headers = {});
    /// Create a response without a body.
    /// @param status
    /// @param headers
    Response(Status status, Headers headers = {});

    /// We own 'body'
    ~Response();

    /// Print this Response to a stream.
    /// @param os
    void print(ostream& os) const;

  private:
    /// Helper funciton.
    static string to_string(Response::Status status);
};

};