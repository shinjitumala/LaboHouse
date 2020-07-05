/// @file Users.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the Users tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <labo/LaboHouse.h>
#include <labo/debug/Log.h>
#include <labo/house/Users.h>

namespace labo {
const Chat::Msg&
LaboHouse::chat(User& user, string msg)
{
    return main_chat.chat(user, msg);
}

const Chat&
LaboHouse::get_main_chat() const
{
    return main_chat;
};
};