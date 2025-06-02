// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <string>
#include <string_view>

class ChatClient
{

public:
  ChatClient(boost::asio::io_context& io, std::string_view host, std::string_view port);
  boost::asio::awaitable<void> send_message(std::string_view endpoint, std::string message);

private:
  boost::asio::ip::tcp::resolver _resolver;
  boost::beast::tcp_stream       _stream;
  std::string_view               _host, _port;
};