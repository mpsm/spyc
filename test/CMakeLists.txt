add_executable(testall
               CodeModel_test.cc
               CodeVisitor_test.cc
               Method_test.cc
               test.cc
               utils_test.cc
               ../src/CodeModel.cc
               ../src/CodeVisitor.cc
               ../src/Method.cc
)

# add clang tooling
target_include_directories(testall PUBLIC ${CLANG_INCLUDE_DIRS})
target_link_libraries(testall clangTooling)

# mark unittest compilation
target_compile_definitions(testall PUBLIC -DUNITTEST=1)

# include files from master source directory
target_include_directories(testall PUBLIC ../src)

# find googletest
find_package(GTest 1.10 CONFIG REQUIRED HINTS ${GTest_HINT})
message(STATUS "Using GTest installation: ${GTest_DIR}")

# use googletest includes and libs
target_include_directories(testall PUBLIC GTest::gtest GTest::gmock)
target_link_libraries(testall GTest::gtest GTest::gmock)

# generate coverage information
if(CMAKE_BUILD_TYPE MATCHES Debug)
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(testall PUBLIC -coverage)
    target_link_libraries(testall -lgcov)
  endif()
  # TODO: generate coverage for clang
endif()

# use googletest test discovery
include(GoogleTest)
gtest_discover_tests(testall WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})