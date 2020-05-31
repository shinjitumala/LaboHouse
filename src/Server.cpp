/// @file Server.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <labo/Server.h>
#include <labo/debug/Log.h>
#include <sys/socket.h>

namespace labo {
Server::Server()
{
    auto socket_fd{::socket(AF_INET, SOCK_STREAM,0)};
    if(socket_fd < 0){
        
    }
};
};