// SPDX-License-Identifier: BSD-2-Clause
#include "http/http_server.hpp"
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
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
          ChatServer server{io, 8080};
          co_await server.start();
        },
        asio::detached);
    std::cout << "Coroutine-based HTTP server running at http://localhost:8080\n";
    io.run();
  }
  catch (const std::exception& e)
  {
    std::cerr << "Fatal error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  return 0;
}