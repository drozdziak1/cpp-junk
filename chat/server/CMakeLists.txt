find_package(Boost 1.58.0 REQUIRED COMPONENTS system regex thread)
find_package(Threads REQUIRED)

add_executable(chatserv main.cpp)

target_link_libraries(chatserv ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
