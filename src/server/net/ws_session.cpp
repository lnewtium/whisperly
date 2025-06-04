// SPDX-License-Identifier: BSD-2-Clause
#include "ws_session.hpp"
#include "tcp_server.hpp"
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <iostream>
#include <utility>

namespace net   = boost::asio;
namespace beast = boost::beast;
using tcp       = boost::asio::ip::tcp;

WSSession::WSSession(tcp::socket socket, std::shared_ptr<TCPServer> server)
    : _ws{std::move(socket)}, _server{std::move(server)}
{
}

auto WSSession::run() -> net::awaitable<void>
{
  try
  {
    std::cout << "Accepting new WebSocket connection...\n";
    co_await _ws.async_accept(net::use_awaitable);
    std::cout << "Handshake successful!\n";
    _server->join(shared_from_this());
    co_await do_read();
  }
  catch (const std::exception& e)
  {
    std::cerr << "Session exception: " << e.what() << '\n';
  }
  _server->leave(shared_from_this());
}

auto WSSession::do_read() -> net::awaitable<void>
{
  for (;;)
  {
    try
    {
      beast::flat_buffer buf;
      co_await _ws.async_read(buf, net::use_awaitable);
      std::string msg = beast::buffers_to_string(buf.data());
      std::cout << "Readed from client: " << msg << '\n';
      // Broadcast to all other sessions via server
      _server->broadcast(msg, shared_from_this());
    }
    catch (const std::exception& e)
    {
      std::cerr << "WebSocket read exception: " << e.what() << '\n';
      break;
    }
  }
}

auto WSSession::deliver(std::string msg) -> void
{
  _msg_hist.push_back(std::move(msg));
  net::co_spawn(_ws.get_executor(), do_write(), net::detached);
}

auto WSSession::do_write() -> net::awaitable<void>
{
  std::cout << "Check for message history...\n";
  while (!_msg_hist.empty())
  {
    std::string msg = std::move(_msg_hist.front());
    _msg_hist.pop_front();
    co_await _ws.async_write(net::buffer(msg), net::use_awaitable);
  }
}