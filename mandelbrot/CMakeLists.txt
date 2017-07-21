find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(mandelbrot main.cpp)

target_link_libraries(mandelbrot glfw ${OPENGL_LIBRARIES})
