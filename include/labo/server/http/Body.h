/// @file Body.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#if __has_include(<filesystem>)
#include <filesystem>
#else
#include <experimental/filesystem>
#endif
#include <fstream>
#include <utility>

namespace labo::http {
class Response;
using namespace std;
class Body
{
    /// Allow response to touch the internal 'ifs'.
    friend Response;
#if __has_include(<filesystem>)
    /// Alias for convenience.
    using path = filesystem::path;
#else
    /// Alias for convenience.
    using path = experimental::filesystem::path;
#endif

    /// stream of the opened file.
    ifstream ifs;

  public:
    /// @param file Where the html file is.
    Body(path file);
    /// Allow move.
    Body(Body&&) = default;

  private:
    /// Disallow copy because we own 'ifs'.
    Body(const Body&) = delete;
};

};