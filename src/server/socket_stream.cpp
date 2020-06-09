/// @file socket_stream.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <algorithm>
#include <labo/server/socket_stream.h>
#include <unistd.h>

namespace labo::socket {
fdstreambuf::fdstreambuf(int file_descriptor)
  : file_descriptor{ file_descriptor }
{
    setg(input_buf, input_buf, input_buf);
    setp(output_buf, output_buf + bufsize - 1);
}

int
fdstreambuf::overflow(int c)
{
    if (!traits_type::eq_int_type(c, traits_type::eof())) {
        *pptr() = traits_type::to_char_type(c);
        pbump(1);
    }
    if (sync() == -1) {
        return traits_type::eof();
    }

    return traits_type::not_eof(c);
}

int
fdstreambuf::sync()
{
    while (pbase() != pptr()) {
        auto data_size{ pptr() - pbase() };
        auto sent_size{ ::write(file_descriptor, output_buf, data_size) };

        if (0 < sent_size) {
            copy(pbase() + sent_size, pptr(), pbase());
            setp(pbase(), epptr());
            pbump(data_size - sent_size);
        }
    }

    if (pptr() == epptr()) {
        return -1;
    }
    return 0;
}

int
fdstreambuf::underflow()
{
    if (gptr() == egptr()) {
        auto pback(std::min(gptr() - eback(), ptrdiff_t(16 - sizeof(int))));
        copy(egptr() - pback, egptr(), eback());
        auto read_size(::read(file_descriptor, eback() + pback, bufsize));
        setg(eback(), eback() + pback, eback() + pback + max(0L, read_size));
    }

    if (gptr() == egptr()) {
        return traits_type::eof();
    }

    return traits_type::to_int_type(*gptr());
}
};