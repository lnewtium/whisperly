// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include <boost/asio/awaitable.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/websocket/stream.hpp>
#include <deque>
#include <memory>

class TCPServer;

/*
 * Handles signle user with beast ws stream over asio tcp socket
 */
class WSSession : public std::enable_shared_from_this<WSSession>
{
public:
  WSSession(boost::asio::ip::tcp::socket socket, std::shared_ptr<TCPServer> server);
  auto run() -> boost::asio::awaitable<void>;
  auto deliver(std::string msg) -> void;

private:
  // FIXME: Enable deflate support?
  boost::beast::websocket::stream<boost::beast::tcp_stream, false> _ws;
  std::deque<std::string>                                          _msg_hist;
  std::shared_ptr<TCPServer>                                       _server;
  // Handles inbound message
  auto do_read() -> boost::asio::awaitable<void>;
  auto do_write() -> boost::asio::awaitable<void>;
};