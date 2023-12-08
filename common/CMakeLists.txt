add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/aoc.cpp"
  COMMAND "${AOC_RE2C_EXEC}" "-W" "-o" "${CMAKE_CURRENT_BINARY_DIR}/aoc.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/aoc.re.cpp"
  DEPENDS re2c
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/aoc.re.cpp"
  WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
  )

add_library(aoc_common
  "${CMAKE_CURRENT_BINARY_DIR}/aoc.cpp"
  hash.cpp
  segmented_range.cpp
  string_tools.cpp
)
target_compile_features(aoc_common PUBLIC cxx_std_17)
target_include_directories(aoc_common PUBLIC common)
target_link_libraries(aoc_common PUBLIC whereami)

add_executable(test_segmented_range test_segmented_range.cpp)
target_link_libraries(test_segmented_range PUBLIC aoc_common)


add_executable(test_string_tools test_string_tools.cpp)
target_link_libraries(test_string_tools PUBLIC aoc_common)

add_executable(test_ndarray test_ndarray.cpp)
target_link_libraries(test_ndarray PUBLIC aoc_common)


add_executable(test_graph_tools test_graph_tools.cpp)
target_link_libraries(test_graph_tools PUBLIC aoc_common)
