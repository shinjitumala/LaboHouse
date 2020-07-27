/// @file Image.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the SWORD tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <filesystem>
#include <fstream>

namespace labo::http {
using namespace std;
using namespace filesystem;

struct Image
{
    path file;
};
}