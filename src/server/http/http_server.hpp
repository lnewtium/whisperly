// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

/*
 * TCP Socket manager
 * Constructs HTTPSession per client
 */
class ChatServer
{
public:
  ChatServer(boost::asio::io_context& io, uint16_t port);
  auto start() -> boost::asio::awaitable<void>;

private:
  void                           handle_connection(boost::asio::ip::tcp::socket socket);
  boost::asio::ip::tcp::acceptor _acceptor;
};