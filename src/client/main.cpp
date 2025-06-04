// SPDX-License-Identifier: BSD-2-Clause
#include "cli/cli.hpp"
#include "net/net_client.hpp"
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <iostream>

namespace net = boost::asio;

auto main() -> int
{
  try
  {
    // TODO: Run ui and net in separate threads
    net::io_context io{1};

    net::co_spawn(
        io,
        [&io]() -> net::awaitable<void>
        {
          auto net_client = std::make_shared<NetClient>(io, "localhost", "8080");
          co_await net_client->handshake();
          net_client->listen_server();
          CLI cli{io, net_client};
          co_await cli.handle_input();
        },
        net::detached);
    io.run();
  }
  catch (const std::exception& e)
  {
    std::cerr << "Fatal error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  return 0;
}