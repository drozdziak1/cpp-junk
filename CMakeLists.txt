cmake_minimum_required (VERSION 3.1)
project (CppJunk)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

subdirs(
bigfac
chat
collatz
confour
galeshapely
letfreqs
mandelbrot
rabinkarp
segtree
strfreqs
timefetch
vowelcount
)
