find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(mandelbrot main.cpp)

target_link_libraries(mandelbrot ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
