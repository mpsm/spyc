#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Frontend/FrontendActions.h>
#include <llvm/Support/CommandLine.h>

#include <iostream>

using namespace clang::tooling;
using namespace llvm;

static cl::OptionCategory category("SpyC");

int main(int argc, const char **argv)
{
    std::cout << "SpyC - another C static analyzer" << std::endl;

    CommonOptionsParser optionParser(argc, argv, category);
    ClangTool tool(optionParser.getCompilations(), optionParser.getSourcePathList());

    return tool.run(newFrontendActionFactory<clang::SyntaxOnlyAction>().get());
}