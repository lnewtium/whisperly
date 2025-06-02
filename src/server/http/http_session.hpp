// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ip/tcp.hpp>

/*
 * Handles signle user with boost::asio::socket
 */
class HTTPSession
{
public:
  HTTPSession(boost::asio::ip::tcp::socket socket);
  auto run() -> boost::asio::awaitable<void>;

private:
  boost::asio::ip::tcp::socket _socket;
};