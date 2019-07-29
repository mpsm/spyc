
#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <llvm/Support/CommandLine.h>

#include <iostream>

#include "CodeConsumer.hh"
#include "CodeModel.hh"

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

int main(int argc, const char **argv)
{
    int err;

    spyc::CodeModel model;
    std::cout << "SpyC - another C static analyzer" << std::endl;

    clang::tooling::CommonOptionsParser optionParser(argc, argv, category);
    clang::tooling::ClangTool tool(optionParser.getCompilations(), optionParser.getSourcePathList());

    err = tool.run(std::make_unique<SpycFrontendActionFactory>(model).get());

    if (err != 0) {
        return err;
    }

    std::cout << "Dumping functions" << std::endl;
    for (auto fp : model.getFunctions()) {
        auto f = fp.second;
        std::cout << fp.first << std::endl;
        std::cout << "  Callers:" << std::endl;
        for (auto weak_caller : f->getCallers()) {
            auto caller = weak_caller.lock();
            std::cout << "    " << caller->getName() << std::endl;
        }

        std::cout << "  Callees:" << std::endl;
        for (auto weak_callee : f->getCallees()) {
            auto callee = weak_callee.lock();
            std::cout << "    " << callee->getName() << std::endl;
        }
    }

    return 0;
}