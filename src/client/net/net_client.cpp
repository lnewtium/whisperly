// SPDX-License-Identifier: BSD-2-Clause
#include "net_client.hpp"
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <iostream>

using namespace boost;
using namespace beast;

constexpr unsigned HTTP_VERSION = 11;

NetClient::NetClient(asio::io_context& io, std::string_view host, std::string_view port)
    : _resolver{asio::make_strand(io)}, _stream{asio::make_strand(io)}, _host{host}, _port{port}
{
}

auto NetClient::handshake() -> boost::asio::awaitable<void>
{
  try
  {
    // Endpoint lookup
    auto server_endpoints = co_await _resolver.async_resolve(_host, _port, asio::use_awaitable);

    // Init http connection
    co_await asio::async_connect(_stream.next_layer().socket(), server_endpoints,
                                 asio::use_awaitable);

    // Perform ws handshake (upgrade http to ws)
    co_await _stream.async_handshake(_host, "/", asio::use_awaitable);
  }
  catch (const std::exception& e)
  {
    std::cerr << "Initial error: " << e.what() << '\n';
  }
}

auto NetClient::listen_server() -> void
{
  asio::co_spawn(
      _stream.get_executor(),
      [this]() -> asio::awaitable<void>
      {
        beast::flat_buffer buf;
        for (;;)
        {
          try
          {
            co_await _stream.async_read(buf, asio::use_awaitable);
            std::cout << "Received: " << beast::buffers_to_string(buf.data()) << '\n' << std::flush;
            buf.consume(buf.size());
          }
          catch (const std::exception& e)
          {
            std::cerr << "Server read error: " << e.what() << '\n';
          }
        }
      },
      asio::detached);
}

auto NetClient::send_message(std::string_view endpoint, std::string msg) -> asio::awaitable<void>
{
  try
  {
    co_await _stream.async_write(asio::buffer(msg), asio::use_awaitable);
  }
  catch (const std::exception& e)
  {
    std::cerr << "WS Write error: " << e.what() << '\n';
  }
}