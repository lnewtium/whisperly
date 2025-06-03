// SPDX-License-Identifier: BSD-2-Clause
#include "tcp_server.hpp"
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/ip/v6_only.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/use_awaitable.hpp>

using namespace boost;
using tcp = asio::ip::tcp;

TCPServer::TCPServer(asio::io_context& io, uint16_t port) : _acceptor{asio::make_strand(io)}
{
  _acceptor.open(tcp::v6());                      // Use dual-stack (ipv4+ipv6 socket)
  _acceptor.set_option(asio::ip::v6_only(false)); // Enable dual-stack
  _acceptor.set_option(tcp::socket::reuse_address{true});
  _acceptor.bind(tcp::endpoint{asio::ip::make_address("::"), port}); // Bind to any ip
  _acceptor.listen();
}

auto TCPServer::start() -> asio::awaitable<void>
{
  // Infinite loop for accepting tcp connections
  for (;;)
  {
    handle_connection(co_await _acceptor.async_accept(asio::use_awaitable));
  }
}

auto TCPServer::handle_connection(boost::asio::ip::tcp::socket socket) -> void
{
  auto session = std::make_shared<WSSession>(std::move(socket), shared_from_this());
  co_spawn(
      _acceptor.get_executor(), [session]() -> asio::awaitable<void> { co_await session->run(); },
      asio::detached);
}

auto TCPServer::join(const std::shared_ptr<WSSession>& session) -> void
{
  _sessions.insert(session);
}

auto TCPServer::leave(const std::shared_ptr<WSSession>& session) -> void
{
  _sessions.erase(session);
}

auto TCPServer::broadcast(const std::string& msg, const std::shared_ptr<WSSession>& sender) -> void
{
  for (auto& session : _sessions)
  {
    if (session != sender)
    {
      session->deliver(msg);
    }
  }
}