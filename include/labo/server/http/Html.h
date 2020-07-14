/// @file Html.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <filesystem>
#include <fstream>
#include <utility>

namespace labo::http {
class Response;
using namespace std;
class Html
{
    /// Allow response to touch the internal 'ifs'.
    friend Response;
    /// Alias for convenience.
    using path = filesystem::path;

    /// Location of the HTML file.
    path file;

  public:
    /// @param file Where the html file is.
    Html(path file);

    /// Creates a new ifstream for the file.
    /// @return ifstream
    ifstream read() const;
};

};