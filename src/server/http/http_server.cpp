// SPDX-License-Identifier: BSD-2-Clause
#include "http_server.hpp"
#include "http_session.hpp"
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/ip/v6_only.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/use_awaitable.hpp>

using namespace boost;
using tcp = asio::ip::tcp;

ChatServer::ChatServer(asio::io_context& io, uint16_t port) : _acceptor{asio::make_strand(io)}
{
  _acceptor.open(tcp::v6());                      // Use dual-stack (ipv4+ipv6 socket)
  _acceptor.set_option(asio::ip::v6_only(false)); // Enable dual-stack
  _acceptor.set_option(tcp::socket::reuse_address{true});
  _acceptor.bind(tcp::endpoint{asio::ip::make_address("::"), port}); // Bind to any ip
  _acceptor.listen();
}

auto ChatServer::start() -> asio::awaitable<void>
{
  // Infinite loop for accepting tcp connections
  for (;;)
  {
    handle_connection(co_await _acceptor.async_accept(asio::use_awaitable));
  }
}

void ChatServer::handle_connection(boost::asio::ip::tcp::socket socket)
{
  auto session = std::make_shared<HTTPSession>(std::move(socket));
  co_spawn(
      _acceptor.get_executor(), [session]() -> asio::awaitable<void> { co_await session->run(); },
      asio::detached);
}