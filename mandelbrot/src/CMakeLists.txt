find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(mandelbrot main.cpp)

message(STATUS "Test: " ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(
  mandelbrot
  PRIVATE
  "${PROJECT_SOURCE_DIR}/mandelbrot/include"
  )

target_link_libraries(mandelbrot glfw ${OPENGL_LIBRARIES})
