cmake_minimum_required(VERSION 3.15)
# so MSVC will compile and link correctly
cmake_policy(SET CMP0091 NEW)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(CMakeDependentOption)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

project(AOC LANGUAGES C CXX)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_DATAROOTDIR}/cmake/aoc)

# build flags
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

if(NOT CMAKE_CONFIGURATION_TYPES)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

##
# @brief Symbolically links a list of files to build directory
# @param ARGN files to link. Must be relative paths!
function(SYMLINK_FILES)
  # make absolute paths
  foreach(s IN LISTS ARGN)
    if(NOT IS_ABSOLUTE "${s}")
      get_filename_component(sabs "${s}" ABSOLUTE)
	  file(CREATE_LINK "${sabs}" "${CMAKE_CURRENT_BINARY_DIR}/${s}" SYMBOLIC)
	else()
	  message(ERROR "can't call SYMLINK_FILES with an absolute path")
    endif()
  endforeach()
endfunction()

function(AOC_EXECUTABLE name)
  # join the 2 parent directories of executable
  get_filename_component(DAY_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
  get_filename_component(YEAR_DIR "${DAY_DIR}" DIRECTORY)
  get_filename_component(YEAR_DIR "${DAY_DIR}" NAME)
  get_filename_component(DAY_DIR "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
  add_executable("${YEAR_DIR}_${DAY_DIR}_${name}" ${ARGN})
  target_link_libraries("${YEAR_DIR}_${DAY_DIR}_${name}" aoc_common)
  set_target_properties("${YEAR_DIR}_${DAY_DIR}_${name}" PROPERTIES OUTPUT_NAME "${name}")
endfunction()

# add user modules
include("aoc-modules")

add_subdirectory(common)

add_subdirectory(2020)

