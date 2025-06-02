find_package(Boost REQUIRED COMPONENTS beast asio)
add_executable(whisperly_client main.cpp
                                http/http_client.hpp
                                http/http_client.cpp)
target_link_libraries(whisperly_client PRIVATE Boost::asio Boost::beast)