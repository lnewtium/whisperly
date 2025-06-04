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

namespace net   = boost::asio;
namespace beast = boost::beast;

constexpr unsigned HTTP_VERSION = 11;

NetClient::NetClient(net::io_context& io, std::string_view host, std::string_view port)
    : _resolver{net::make_strand(io)}, _stream{net::make_strand(io)}, _host{host}, _port{port}
{
}

auto NetClient::handshake() -> net::awaitable<void>
{
  try
  {
    // Endpoint lookup
    auto server_endpoints = co_await _resolver.async_resolve(_host, _port, net::use_awaitable);

    // Init http connection
    co_await net::async_connect(_stream.next_layer().socket(), server_endpoints,
                                net::use_awaitable);

    // Perform ws handshake (upgrade http to ws)
    co_await _stream.async_handshake(_host, "/", net::use_awaitable);
  }
  catch (const std::exception& e)
  {
    std::cerr << "Initial error: " << e.what() << '\n';
  }
}

auto NetClient::listen_server() -> void
{
  net::co_spawn(
      _stream.get_executor(),
      [this]() -> net::awaitable<void>
      {
        beast::flat_buffer buf;
        for (;;)
        {
          try
          {
            co_await _stream.async_read(buf, net::use_awaitable);
            std::cout << "Received: " << beast::buffers_to_string(buf.data()) << '\n' << std::flush;
            buf.consume(buf.size());
          }
          catch (const std::exception& e)
          {
            std::cerr << "Server read error: " << e.what() << '\n';
          }
        }
      },
      net::detached);
}

auto NetClient::send_message(std::string_view endpoint, std::string msg) -> net::awaitable<void>
{
  try
  {
    co_await _stream.async_write(net::buffer(msg), net::use_awaitable);
  }
  catch (const std::exception& e)
  {
    std::cerr << "WS Write error: " << e.what() << '\n';
  }
}