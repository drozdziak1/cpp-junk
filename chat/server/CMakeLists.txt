find_package(Boost 1.58.0 REQUIRED COMPONENTS system regex)

add_executable(chatserv main.cpp)

target_link_libraries(chatserv ${Boost_LIBRARIES})
