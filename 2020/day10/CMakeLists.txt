SYMLINK_FILES(input.txt)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/part1.cpp"
  COMMAND "${AOC_RE2C_EXEC}" "-W" "-o" "${CMAKE_CURRENT_BINARY_DIR}/part1.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/part1.re.cpp"
  DEPENDS re2c
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/part1.re.cpp"
  WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
  )

add_executable(day10_part1_2020 "${CMAKE_CURRENT_BINARY_DIR}/part1.cpp")
target_link_libraries(day10_part1_2020 aoc_common)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/part2.cpp"
  COMMAND "${AOC_RE2C_EXEC}" "-W" "-o" "${CMAKE_CURRENT_BINARY_DIR}/part2.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/part2.re.cpp"
  DEPENDS re2c
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/part2.re.cpp"
  WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
  )

add_executable(day10_part2_2020 "${CMAKE_CURRENT_BINARY_DIR}/part2.cpp")
target_link_libraries(day10_part2_2020 aoc_common)
