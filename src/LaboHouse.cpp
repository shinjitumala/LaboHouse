/// @file LaboHouse.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <labo/LaboHouse.h>
#include <labo/debug/Log.h>

namespace labo {
ulong
LaboHouse::add_name(string name)
{
    static ulong name_counter{ 0 };
    lock_guard<mutex> lg{ mtx_names };
    if (name_to_id.count(name)) {
        return get_id(name);
    }
    name_to_id.insert({ name, name_counter });
    id_to_name.insert({ name_counter, name });
    auto id{ name_counter };
    logs << "New user: id = " << id << ", name = " << name << endl;
    name_counter++;
    return id;
}

bool
LaboHouse::name_exists(string name) const
{
    return name_to_id.count(name);
}

ulong
LaboHouse::get_id(string name) const
{
    return name_to_id.at(name);
}

string
LaboHouse::get_name(ulong id) const
{
    return id_to_name.at(id);
}

void
foo(const LaboHouse& a)
{
    const LaboHouse lh;
    // lh.add_name("foo");
}
};