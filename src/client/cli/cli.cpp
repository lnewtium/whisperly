// SPDX-License-Identifier: BSD-2-Clause
#include "cli.hpp"
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/use_awaitable.hpp>

using namespace boost;

CLI::CLI(asio::io_context& io, std::shared_ptr<NetClient> net)
    : _strand{asio::make_strand(io)}, _net{std::move(net)}
{
}

auto CLI::handle_input() -> boost::asio::awaitable<void>
{
  // Wrap stdin (fd 0) into Asio stream_descriptor
  asio::posix::stream_descriptor stream_in(_strand, ::dup(STDIN_FILENO));
  for (;;)
  {
    asio::streambuf buf;
    size_t          n = co_await asio::async_read_until(stream_in, buf, '\n', asio::use_awaitable);
    std::istream    is(&buf);
    std::string     msg;
    std::getline(is, msg);
    if (msg == "/exit")
      break;
    co_await _net->send_message("/", msg);
  }
}