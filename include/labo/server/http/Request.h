/// @file http.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <labo/util/OptionalRef.h>
#include <labo/util/stream.h>
#include <map>
#include <string>

namespace labo::http {
using namespace std;

/// Represents a HTTP request
class Request
{
  public:
    /// Request method.
    enum class Method
    {
        GET,
        POST,
    };

    /// Set this from stream.
    /// @param is
    void deserialize(istream& is);

    /// Obtain the request method.
    /// @return Method
    Method method() const;

    /// Obtain the request path.
    /// @return const string
    const string path() const;

    /// Attempts to get a value from the query string.
    /// @param value_name
    /// @return OptionalRef<string>
    OptionalRef<const string> query_value(const string value_name) const;
    /// Attempts to get a value from the header.
    /// @param value_name
    /// @return OptionalRef<string>
    OptionalRef<const string> header_value(const string value_name) const;

  private:
    /// Method of the request
    Method method_tag;
    /// Path of the request
    string raw_path;
    /// body
    string body;

    /// All value data from query string
    map<string, string> query;
    /// All header data
    map<string, string> headers;
};
};