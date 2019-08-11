#ifndef __SPYC_SPYCFRONTENDACTION_HH__
#define __SPYC_SPYCFRONTENDACTION_HH__

#include "CodeConsumer.hh"
#include "CodeModel.hh"

#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>

#include <llvm/ADT/StringRef.h>

#include <memory>

namespace spyc {

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

}

#endif /* __SPYC_SPYCFRONTENDACTION_HH__ */