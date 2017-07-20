set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/")

find_package(GMP REQUIRED)

add_executable(bigfac main.cpp)
target_link_libraries(bigfac ${GMP_LIBRARIES})
