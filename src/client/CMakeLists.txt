find_package(Boost REQUIRED COMPONENTS beast asio)
add_executable(whisperly_client main.cpp
                                net/net_client.hpp
                                net/net_client.cpp
                                cli/cli.hpp
                                cli/cli.cpp)
target_link_libraries(whisperly_client PRIVATE Boost::asio Boost::beast)