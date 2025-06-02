// SPDX-License-Identifier: BSD-2-Clause
#include "http_session.hpp"
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http.hpp>
#include <iostream>

using namespace boost;
namespace http = beast::http;
using tcp      = asio::ip::tcp;

HTTPSession::HTTPSession(tcp::socket socket) : _socket{std::move(socket)} {}

auto HTTPSession::run() -> asio::awaitable<void>
{
  try
  {
    beast::flat_buffer                buf;
    http::request<http::string_body>  req;
    http::response<http::string_body> res;

    co_await http::async_read(_socket, buf, req, asio::use_awaitable);

    // Sample response
    res.version(req.version());
    res.keep_alive(false);
    res.result(http::status::ok);
    res.set(http::field::server, "Whisperly server");
    res.set(http::field::content_type, "text/plain");
    res.body() = req.body();
    res.prepare_payload();

    co_await http::async_write(_socket, res, asio::use_awaitable);
    _socket.shutdown(tcp::socket::shutdown_send);
  }
  catch (const std::exception& e)
  {
    std::cerr << "Session exception: " << e.what() << '\n';
  }
}