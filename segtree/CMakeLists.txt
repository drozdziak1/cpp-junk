add_executable(segtree src/main.cpp src/SegTree.cpp)

target_include_directories(
  segtree
  PRIVATE
  "include"
  )
