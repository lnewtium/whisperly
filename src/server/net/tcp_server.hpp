// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "ws_session.hpp"
#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <unordered_set>

/*
 * TCP Socket manager
 * Constructs WSSession per client
 */
class TCPServer : public std::enable_shared_from_this<TCPServer>
{
public:
  TCPServer(boost::asio::io_context& io, uint16_t port);
  auto start() -> boost::asio::awaitable<void>;
  // Called by WSSession to broadcast a message to all except the sender
  auto broadcast(const std::string& msg, const std::shared_ptr<WSSession>& sender) -> void;

  // Called by WSSession when it starts
  auto join(const std::shared_ptr<WSSession>& session) -> void;

  // Called by WSSession when it stops
  auto leave(const std::shared_ptr<WSSession>& session) -> void;

private:
  auto handle_connection(boost::asio::ip::tcp::socket socket) -> void;
  std::unordered_set<std::shared_ptr<WSSession>> _sessions;
  boost::asio::ip::tcp::acceptor                 _acceptor;
};