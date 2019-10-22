add_executable(testall
               test.cc
               CodeModel_test.cc
               Method_test.cc
               utils_test.cc
               ../src/CodeModel.cc
               ../src/Method.cc
)

# include Clang relataed tests
# this option allows to turn off Clang related tests and reduce
# compilation time

if(NOT DEFINED SPYC_CLANG_TESTS)
  set(SPYC_CLANG_TESTS ON)
endif()
if(SPYC_CLANG_TESTS)
  message(STATUS "Clang related tests on")
  target_sources(testall
    PUBLIC
      ../src/CodeVisitor.cc
      CodeVisitor_test.cc
  )
  # add clang tooling
  target_include_directories(testall PUBLIC ${CLANG_INCLUDE_DIRS})
  target_link_libraries(testall clangTooling)
endif()

# mark unittest compilation
target_compile_definitions(testall PUBLIC -DUNITTEST=1)

# include files from master source directory
target_include_directories(testall PUBLIC ../src)

# find googletest
find_package(GTest 1.10 CONFIG REQUIRED HINTS ${GTest_HINT})
message(STATUS "Using GTest installation: ${GTest_DIR}")

# use googletest's libs
target_link_libraries(testall GTest::gtest GTest::gmock)

# generate coverage information
if(CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_options(testall PUBLIC --coverage)
  target_link_libraries(testall --coverage)
else()
  message(WARNING "Release build type, coverage info is unavailable")
endif()

# use googletest test discovery
include(GoogleTest)
gtest_discover_tests(testall WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
