/// @file http.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <labo/server/http/Html.h>
#include <nlohmann/json.hpp>
#include <labo/util/stream.h>
#include <map>
#include <string>
#include <variant>

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
        UNAURHORIZED = 401,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
    };

  private:
    /// Dummy class used to represent empty body.
    struct None
    {
        explicit None() = default;
    };

    /// Body
    using Body = variant<None, Html, nlohmann::json>;

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
    Body body;

  public:
    /// Construct a response.
    /// @param status The status.
    /// @param body The body.
    /// @param headers Headers. Optional. NOTE: 'Content-Length' is not needed.
    template<class T>
    Response(Status status, T body, Headers headers = {});
    /// Create a response without a body.
    /// @param status
    /// @param headers
    Response(Status status, Headers headers = {});

    /// Print this Response to a stream.
    /// @param os
    void print(ostream& os) const;

  private:
    /// Helper funciton.
    static string to_string(Response::Status status);
};

template<class T>
Response::Response(Status status, T body, Headers headers)
  : status{ status }
  , headers{ headers }
  , body{ body }
{}

};