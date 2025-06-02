find_package(Boost REQUIRED COMPONENTS beast asio)
add_executable(whisperly_server main.cpp
                                http/http_server.hpp
                                http/http_server.cpp
                                http/http_session.hpp
                                http/http_session.cpp)
target_link_libraries(whisperly_server PRIVATE Boost::asio Boost::beast)