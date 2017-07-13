find_package(Boost 1.58.0 REQUIRED COMPONENTS system regex thread)

add_executable(chatclient main.cpp)

target_link_libraries(chatclient ${Boost_LIBRARIES})
