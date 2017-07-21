set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/")

message(STATUS "Module dirs: " ${CMAKE_MODULE_PATH})

find_package(LibDL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(mandelbrot src/main.cpp src/glad.c)

target_include_directories(
  mandelbrot
  PRIVATE
  "include"
  )

target_link_libraries(mandelbrot glfw ${OPENGL_LIBRARIES} ${LIBDL_LIBRARIES})
