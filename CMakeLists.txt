cmake_minimum_required(VERSION 3.10)
project(SpyC)

# relative/absolute paths policy
cmake_policy(SET CMP0076 NEW)

# check compiler version
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "8.0.0")
    set(ERROR_INVALID_COMPILER "GCC")
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0.0")
    set(ERROR_INVALID_COMPILER "Clang")
  endif()
endif()
if(ERROR_INVALID_COMPILER)
  message(FATAL_ERROR "Invalid compiler (${ERROR_INVALID_COMPILER} ${CMAKE_CXX_COMPILER_VERSION}). Use >=Clang9 or >=GCC8 instead.")
endif()

# find clang package
# can't request Clang version, need to check it later
find_package(Clang CONFIG REQUIRED HINTS ${Clang_HINT})
message(STATUS "Using Clang installation: ${Clang_DIR}")
if(NOT LLVM_VERSION VERSION_EQUAL "9.0.0")
  message(FATAL_ERROR "Clang/LLVM version mismatch - found ${LLVM_VERSION}, 9.0.0 expected")
endif()

# check for hardcoded file required by LLVM (possible LLVM bug)
if(NOT EXISTS /usr/local/lib/libz3.so)
  message(FATAL_ERROR "LLVM requires libz3 installed as /usr/local/lib/libz3.s. Create symlink if needed.")
endif()

# turn off optimization for debug builds
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

# use C++17 by default
add_compile_options(-std=c++17)

# don't use RTTI
add_compile_options(-fno-rtti)

# explicetely linking against stdc++
link_libraries(-lstdc++fs -lstdc++)

# build main application
add_subdirectory(src)

# build tests
if(SPYC_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
