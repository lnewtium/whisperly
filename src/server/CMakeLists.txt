find_package(Boost REQUIRED COMPONENTS beast asio)
add_executable(whisperly_server main.cpp
                                net/tcp_server.hpp
                                net/tcp_server.cpp
                                net/ws_session.hpp
                                net/ws_session.cpp)
target_link_libraries(whisperly_server PRIVATE Boost::asio Boost::beast)