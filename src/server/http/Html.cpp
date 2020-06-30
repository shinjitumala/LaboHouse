/// @file Html.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the SWORD tool. Proprietary and confidential.
/// See the licenses directory for details.

#include <filesystem>
#include <fstream>
#include <labo/debug/Log.h>
#include <labo/server/http/Html.h>

namespace labo::http {
Html::Html(path file)
  : file{ file }
{
    if (!filesystem::exists(file)) {
        errs << "File does not exist: " << file << endl;
        failure();
    }
}

ifstream
Html::read() const
{
    return file;
}
};