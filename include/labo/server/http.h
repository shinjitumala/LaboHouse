/// @file http.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <labo/server/Base.h>

namespace labo {
class HttpServer : public Server
{
    /// Alias for convenience.
    using Base = Server;

  public:
    /// Use Server's constructor.
    using Base::Base;

  private:
    static void action(int socket_fd) override;
};
};