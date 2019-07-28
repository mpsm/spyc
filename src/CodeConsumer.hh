#ifndef __SPYC_CODECONSUMER_HH__
#define __SPYC_CODECONSUMER_HH__

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>

#include "CodeVisitor.hh"

namespace spyc {

    class CodeConsumer : public clang::ASTConsumer {
    public:
        explicit CodeConsumer(clang::ASTContext *astContext)
            : visitor(astContext) {

        }

        virtual void HandleTranslationUnit(clang::ASTContext &Context) {
            visitor.TraverseDecl(Context.getTranslationUnitDecl());
        }

    private:
        CodeVisitor visitor;
    };

}

#endif /* __SPYC_CODECONSUMER_HH__ */