
#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/ManagedStatic.h>

#include <iostream>
#include <string>

#include "CodeConsumer.hh"
#include "CodeModel.hh"
#include "DotOutputter.hh"

class SpycFrontendAction : public clang::ASTFrontendAction {
public:
    explicit SpycFrontendAction(spyc::CodeModel& model)
        : _model(model) {}

    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
        return std::unique_ptr<clang::ASTConsumer>(new spyc::CodeConsumer(&Compiler.getASTContext(), _model));
    }

private:
    spyc::CodeModel& _model;
};

class SpycFrontendActionFactory : public clang::tooling::FrontendActionFactory {
public:
    explicit SpycFrontendActionFactory(spyc::CodeModel& model)
        : _model(model) {}

    clang::FrontendAction *create() override {
        return new SpycFrontendAction(_model);
    }
private:
    spyc::CodeModel& _model;
};

static llvm::cl::OptionCategory category("SpyC");

void printVersion(llvm::raw_ostream &os)
{
    static const std::string version = "0.0.1";
    os << "  SpyC version: " << version << "\n";
}

int main(int argc, const char **argv)
{
    int err;
    llvm::llvm_shutdown_obj llvmCleanup;

    spyc::CodeModel model;

    llvm::cl::AddExtraVersionPrinter(printVersion);
    clang::tooling::CommonOptionsParser optionParser(argc, argv, category);
    clang::tooling::ClangTool tool(optionParser.getCompilations(), optionParser.getSourcePathList());

    err = tool.run(std::make_unique<SpycFrontendActionFactory>(model).get());

    if (err != 0) {
        return err;
    }

    {
        spyc::DotOutputter outputter(model);
        outputter.outputCallGraph(std::cout);
    }

    return 0;
}