/// @file LaboHouse.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <labo/house/Users.h>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace labo {
using namespace std;
class LaboHouse : public Users
{

  public:
    LaboHouse() = default;

    /// Dump contents for debugging.
    /// @param os
    void print(ostream& os) const;

  private:
};
};