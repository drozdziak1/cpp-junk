find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(mandelbrot src/main.cpp)

target_include_directories(
  mandelbrot
  PRIVATE
  "include"
  )

target_link_libraries(mandelbrot glfw ${OPENGL_LIBRARIES})
