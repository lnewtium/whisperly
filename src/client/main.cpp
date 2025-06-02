// SPDX-License-Identifier: BSD-2-Clause
#include "http/http_client.hpp"
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
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
          ChatClient http_client{io, "127.0.0.1", "8080"};
          co_await http_client.send_message("/", "sometext");
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