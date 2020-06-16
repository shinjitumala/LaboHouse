/// @file Body.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the SWORD tool. Proprietary and confidential.
/// See the licenses directory for details.

#include <labo/debug/Log.h>
#include <labo/server/http/Body.h>

namespace labo::http {
Body::Body(path file)
  : ifs{ file }
{
    if (!ifs.is_open()) {
        errs << "Failed to open file: " << file << endl;
        failure();
    }
}
};