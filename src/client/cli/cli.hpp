// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "../net/net_client.hpp"
#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <memory>

class CLI
{
public:
  CLI(boost::asio::io_context& io, std::shared_ptr<NetClient> net);
  auto handle_input() -> boost::asio::awaitable<void>;

private:
  boost::asio::strand<boost::asio::io_context::executor_type> _strand;
  std::shared_ptr<NetClient>                                  _net;
};