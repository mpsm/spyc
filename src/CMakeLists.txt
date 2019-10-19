# generate Git version file
add_custom_command(
  OUTPUT
    gitversion.cc
  COMMAND
    ${PROJECT_SOURCE_DIR}/devtools/gitversion.sh gitversion.cc
  DEPENDS
    ${PROJECT_SOURCE_DIR}/.git/HEAD
    ${PROJECT_SOURCE_DIR}/.git/index
  COMMENT
    "Generating VCS information"
)

add_executable(spyc
               CallGraph.cc
               CallGraph.hh
               CodeConsumer.hh
               CodeModel.cc
               CodeModel.hh
               CodeVisitor.cc
               CodeVisitor.hh
               DotOutputter.hh
               gitversion.cc
               gitversion.hh
               Method.cc
               Method.hh
               spyc.cc
               SpycFrontendAction.hh
               SpycFrontendFactory.hh
               utils.hh
)

target_include_directories(spyc PUBLIC ${CLANG_INCLUDE_DIRS})
target_link_libraries(spyc clangTooling)