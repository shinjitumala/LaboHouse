/// @file socket_stream.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <streambuf>
namespace labo::socket {
using namespace std;

class fdstreambuf : public streambuf
{
    /// Alias for convenience
    using traits_type = streambuf::traits_type;

    /// Buffer size for both input and output.
    static const int bufsize{ 1024 };
    /// Output buffer
    char output_buf[bufsize];
    /// Input buffer
    char input_buf[bufsize + 16 - sizeof(int)];
    /// Filedescriptor this is mapped to.
    int file_descriptor;

  public:
    /// @param file_descriptor The file_descriptor this will be representing.
    /// WARNING: fdstreambuf will not take ownership of the 'file_descriptor'.
    /// You still need to close it manually.
    fdstreambuf(int file_descriptor);

  protected:
    int overflow(int c);
    int underflow();
    int sync();

  private:
};
};