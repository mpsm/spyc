
#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <llvm/Support/CommandLine.h>

#include <iostream>

#include "CodeConsumer.hh"

class SpycFrontendAction : public clang::ASTFrontendAction {
public:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
        return std::unique_ptr<clang::ASTConsumer>(new spyc::CodeConsumer(&Compiler.getASTContext()));
    }
};

static llvm::cl::OptionCategory category("SpyC");

int main(int argc, const char **argv)
{
    std::cout << "SpyC - another C static analyzer" << std::endl;

    clang::tooling::CommonOptionsParser optionParser(argc, argv, category);
    clang::tooling::ClangTool tool(optionParser.getCompilations(), optionParser.getSourcePathList());

    return tool.run(clang::tooling::newFrontendActionFactory<SpycFrontendAction>().get());
}