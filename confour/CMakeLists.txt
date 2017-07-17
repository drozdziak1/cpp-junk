set(CURSES_NEED_NCURSES, TRUE)
find_package(Curses REQUIRED)

add_executable(confour main.cpp game.cpp)

include_directories(".")
target_link_libraries(confour ${CURSES_LIBRARIES})
