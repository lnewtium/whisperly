// SPDX-License-Identifier: BSD-2-Clause
#include "net/net_client.hpp"
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <iostream>

using namespace boost;

int main()
{
  try
  {
    asio::io_context io{1};

    asio::co_spawn(
        io,
        [&io]() -> asio::awaitable<void>
        {
          ChatClient net_client{io, "localhost", "8080"};
          co_await net_client.handshake();
          net_client.listenServer();
          // Wrap stdin (fd 0) into Asio stream_descriptor
          asio::posix::stream_descriptor stream_in(io, ::dup(STDIN_FILENO));
          for (;;)
          {
            asio::streambuf buf;
            size_t n = co_await asio::async_read_until(stream_in, buf, '\n', asio::use_awaitable);
            std::istream is(&buf);
            std::string  line;
            std::getline(is, line);
            co_await net_client.send_message("/", line);
          }
        },
        asio::detached);
    io.run();
  }
  catch (const std::exception& e)
  {
    std::cerr << "Fatal error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  return 0;
}