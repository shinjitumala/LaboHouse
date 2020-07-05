/// @file LaboHouse.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <labo/house/Chat.h>
#include <labo/house/Users.h>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace labo {
using namespace std;
class LaboHouse : public Users
{
    Chat main_chat;

  public:
    LaboHouse() = default;

    /// Dump contents for debugging.
    /// @param os
    void print(ostream& os) const;

    /// Chat to the main chat.
    /// @param user
    /// @param msg
    const Chat::Msg& chat(User& user, string msg);

    /// Obtain main chat for printing.
    /// @return const Chat&
    const Chat& get_main_chat() const;

  private:
};
};