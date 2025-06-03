// SPDX-License-Identifier: BSD-2-Clause
#include "net/tcp_server.hpp"
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <iostream>

using namespace boost;

auto main() -> int
{
  try
  {
    asio::io_context io{1};
    asio::co_spawn(
        io,
        [&io]() -> asio::awaitable<void>
        {
          auto server = std::make_shared<TCPServer>(io, 8080);
          co_await server->start();
        },
        asio::detached);
    std::cout << "Coroutine-based WebSocket server running at http://localhost:8080\n";
    io.run();
  }
  catch (const std::exception& e)
  {
    std::cerr << "Fatal error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  return 0;
}