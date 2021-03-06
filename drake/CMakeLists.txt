cmake_minimum_required(VERSION 3.5.1)
project(drake VERSION 0.0.0)

cmake_policy(SET CMP0025 NEW)
cmake_policy(SET CMP0042 NEW)

include(../cmake/config.cmake)
include(../cmake/install.cmake)
include(../cmake/packages.cmake)
include(../cmake/test.cmake)

drake_setup_platform()
drake_find_packages()

# Create a cache variable that contains the
# max number of concurent link jobs that can be run
# in a Ninja build, if the value is 0 then let Ninja
# use as many as it wants.
set(CMAKE_NINJA_LINK_POOL_SIZE 0 CACHE STRING
  "Number of concurent link jobs that can be run with Ninja build tool")
# if the link pool is non-zero then set the property
# on all the targets in drake
if(CMAKE_NINJA_LINK_POOL_SIZE GREATER 0)
  # Ninja: Configure a job pool to limit simultaneous linking.
  set_property(GLOBAL PROPERTY JOB_POOLS link_pool=${CMAKE_NINJA_LINK_POOL_SIZE})
  # Ninja: Assign all linking to our link job pool.
  set(CMAKE_JOB_POOL_LINK link_pool)
endif()

# options
option(LONG_RUNNING_TESTS "some tests should be run nightly for coverage, but are too slow for CI" OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Doxygen MODULE)
find_package(Threads MODULE REQUIRED)

# This makes all of our #include "drake/..." statements work.
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/..)

# To find generated protobuf headers.
include_directories(BEFORE ${PROJECT_BINARY_DIR}/..)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=all -Werror=ignored-qualifiers -Werror=overloaded-virtual")
if(NOT CMAKE_CXX_FLAGS MATCHES "-fsanitize=") # sanitizers are extremely brittle without lazy linking
  if(APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-undefined -Wl,error ${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS "-Wl,-undefined -Wl,error ${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,-undefined -Wl,error ${CMAKE_EXE_LINKER_FLAGS}")
  else()
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined ${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--no-undefined ${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-undefined ${CMAKE_EXE_LINKER_FLAGS}")
  endif()
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # TODO(#2852) Turn on shadow checking for g++ once we use a version that fixes
  # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=57709
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=shadow")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=inconsistent-missing-override")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=sign-compare")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-stack-address")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=non-virtual-dtor")
else()
  # TODO(jwnimmer-tri) Get a similar complement of flags working on clang.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=extra")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-local-addr")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=non-virtual-dtor")
  # TODO(jwnimmer-tri) Fix these warnings and remove this suppression.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")
  # This is only intended for tests, but is included for the entire build
  # in CMake since there is not an easy mechanism to enable this only for
  # tests.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
  # TODO(#2852) Turn on shadow checking for g++ once we use a version that
  # fixes https://gcc.gnu.org/bugzilla/show_bug.cgi?id=57709
endif()
set(CXX_FLAGS_NO_ERROR_SHADOW -Wno-error=shadow -Wno-shadow)
set(CXX_FLAGS_NO_SIGN_COMPARE -Wno-sign-compare)

# set up matlab build
include(../cmake/mex.cmake)
if(DISABLE_MATLAB AND Matlab_FOUND) # Set by drake_setup_platform
  message(STATUS "MATLAB is disabled because the CMake option DISABLE_MATLAB is set to ON.")
  unset(MATLAB_EXECUTABLE CACHE)
  unset(Matlab_FOUND)
endif()

enable_testing()
include(CTest)
set(GTEST_ROOT ${CMAKE_INSTALL_PREFIX})

add_subdirectory(thirdParty/boost/tcbrindle_cpp17_headers/stx)
add_subdirectory(thirdParty/bsd/spruce)
add_subdirectory(thirdParty/zlib/tinyxml2)
add_subdirectory(common)
add_subdirectory(math)
add_subdirectory(multibody)
add_subdirectory(util)
add_subdirectory(solvers)

add_subdirectory(bindings/python)
add_subdirectory(bindings/pybind11)

if(Matlab_FOUND)
  add_subdirectory(matlab)
endif()

option(ENABLE_DOCUMENTATION "Enable build target for Doxygen and Sphinx documentation." ON)

if(ENABLE_DOCUMENTATION)
  add_subdirectory(doc)
endif()
