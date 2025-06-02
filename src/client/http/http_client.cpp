// SPDX-License-Identifier: BSD-2-Clause
#include "http_client.hpp"
#include <boost/asio/strand.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp> // For BOOST_BEAST_VERSION_STRING
#include <iostream>

using namespace boost;
using namespace beast;

constexpr unsigned HTTP_VERSION = 11;

ChatClient::ChatClient(asio::io_context& io, std::string_view host, std::string_view port)
    : _resolver{asio::make_strand(io)}, _stream{asio::make_strand(io)}, _host{host}, _port{port}
{
}

asio::awaitable<void> ChatClient::send_message(std::string_view endpoint, std::string message)
{
  try
  {
    // DNS lookup
    auto host_ip = co_await _resolver.async_resolve(_host, _port, asio::use_awaitable);

    // Init TCP connect
    co_await _stream.async_connect(host_ip, asio::use_awaitable);

    // Build HTTP request
    http::request<http::string_body> req{http::verb::post, endpoint, HTTP_VERSION};
    req.set(http::field::host, _host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::content_type, "text/plain");
    req.body() = std::move(message);
    req.prepare_payload();

    // Send request
    co_await http::async_write(_stream, req, asio::use_awaitable);

    // Read the response
    beast::flat_buffer                buf;
    http::response<http::string_body> res;
    co_await http::async_read(_stream, buf, res, asio::use_awaitable);

    std::cout << "Server response:\n" << res.body() << "\n";

    // Shutdown
    beast::error_code ec;
    _stream.socket().shutdown(asio::ip::tcp::socket::shutdown_both, ec);
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
  }
}