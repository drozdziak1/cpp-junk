find_package(GLUT REQUIRED)

add_executable(mandelbrot main.cpp)
target_link_libraries(mandelbrot ${GLUT_LIBRARIES})
