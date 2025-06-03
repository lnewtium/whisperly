// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/websocket/stream.hpp>
#include <string>
#include <string_view>

class NetClient
{

public:
  NetClient(boost::asio::io_context& io, std::string_view host, std::string_view port);
  auto handshake() -> boost::asio::awaitable<void>;
  auto send_message(std::string_view endpoint, std::string msg) -> boost::asio::awaitable<void>;
  auto listen_server() -> void;

private:
  boost::asio::ip::tcp::resolver _resolver;
  // FIXME: Enable deflate support?
  boost::beast::websocket::stream<boost::beast::tcp_stream, false> _stream;
  std::string_view                                                 _host, _port;
};