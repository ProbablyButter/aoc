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
# @param ARGN files to link
function(SYMLINK_FILES)
  # make absolute paths
  get_property(is_defined GLOBAL PROPERTY AOC_SYM_SRCS DEFINED)
  if(NOT is_defined)
    define_property(GLOBAL PROPERTY AOC_SYM_SRCS
      BRIEF_DOCS "List of all symlink files to link to build directory"
      FULL_DOCS "List of all symlink files to link to build directory")
  endif()

  get_property(is_defined GLOBAL PROPERTY AOC_SYM_DSTS DEFINED)
  if(NOT is_defined)
    define_property(GLOBAL PROPERTY AOC_SYM_DSTS
      BRIEF_DOCS "List of all symlink files to link to build directory"
      FULL_DOCS "List of all symlink files to link to build directory")
  endif()
  
  set(SRCS)
  set(DSTS)
  foreach(s IN LISTS ARGN)
    list(APPEND DSTS "${CMAKE_CURRENT_BINARY_DIR}/${s}")
    if(NOT IS_ABSOLUTE "${s}")
      get_filename_component(s "${s}" ABSOLUTE)
    endif()
    list(APPEND SRCS "${s}")
  endforeach()
  set_property(GLOBAL APPEND PROPERTY AOC_SYM_SRCS "${SRCS}")
  set_property(GLOBAL APPEND PROPERTY AOC_SYM_DSTS "${DSTS}")
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

function(GENERATE_SYM_LINKS)
    message(STATUS "Generating symbolic links")
    get_property(SRCS GLOBAL PROPERTY AOC_SYM_SRCS)
    get_property(DSTS GLOBAL PROPERTY AOC_SYM_DSTS)
    list(LENGTH SRCS len1)
    if(len1)
        math(EXPR len2 "${len1}-1")
        foreach(var RANGE ${len2})
            list(GET SRCS ${var} src)
            list(GET DSTS ${var} dst)
            add_custom_command(OUTPUT "${dst}"
                COMMAND ln -sf "${src}" "${dst}"
                DEPENDS "${src}"
                )
            set_source_files_properties("${dst}"
                PROPERTIES GENERATED TRUE)
        endforeach()
        add_custom_target(AOC_SYM_LINKS ALL
            DEPENDS ${DSTS})
    endif()
endfunction()

GENERATE_SYM_LINKS()

