/// @file LaboHouse.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <mutex>
#include <string>
#include <unordered_map>

namespace labo {
using namespace std;
class LaboHouse
{
    mutex mtx_names;
    unordered_map<string, ulong> name_to_id;
    unordered_map<ulong, string> id_to_name;

  public:
    LaboHouse() = default;

    /// Adds a new name
    /// @param name
    /// @return ulong the cookie for the newly added name
    ulong add_name(string name);
    ///
    /// @param name
    /// @return ulong
    ulong get_id(string name) const;

    /// 
    /// @param id 
    /// @return string 
    string get_name(ulong id) const;

    ///
    /// @param os
    void print(ostream& os) const;

  private:
};
};