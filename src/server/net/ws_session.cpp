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

using namespace boost;
using tcp = asio::ip::tcp;

WSSession::WSSession(asio::ip::tcp::socket socket, std::shared_ptr<TCPServer> server)
    : _ws{std::move(socket)}, _server{std::move(server)}
{
}

auto WSSession::run() -> asio::awaitable<void>
{
  try
  {
    std::cout << "Accepting new WebSocket connection...\n";
    co_await _ws.async_accept(asio::use_awaitable);
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

auto WSSession::do_read() -> asio::awaitable<void>
{
  for (;;)
  {
    try
    {
      beast::flat_buffer buf;
      co_await _ws.async_read(buf, asio::use_awaitable);
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
  asio::co_spawn(_ws.get_executor(), do_write(), asio::detached);
}

auto WSSession::do_write() -> asio::awaitable<void>
{
  std::cout << "Check for message history...\n";
  while (!_msg_hist.empty())
  {
    std::string msg = std::move(_msg_hist.front());
    _msg_hist.pop_front();
    co_await _ws.async_write(asio::buffer(msg), asio::use_awaitable);
  }
}