// SPDX-License-Identifier: BSD-2-Clause
#include "cli.hpp"
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/use_awaitable.hpp>

namespace net = boost::asio;

CLI::CLI(net::io_context& io, std::shared_ptr<NetClient> net)
    : _strand{net::make_strand(io)}, _net{std::move(net)}
{
}

auto CLI::handle_input() -> net::awaitable<void>
{
  // Wrap stdin (fd 0) into Asio stream_descriptor
  net::posix::stream_descriptor stream_in(_strand, ::dup(STDIN_FILENO));
  for (;;)
  {
    net::streambuf buf;
    size_t         n = co_await net::async_read_until(stream_in, buf, '\n', net::use_awaitable);
    std::istream   is(&buf);
    std::string    msg;
    std::getline(is, msg);
    if (msg == "/exit")
      break;
    co_await _net->send_message("/", msg);
  }
}