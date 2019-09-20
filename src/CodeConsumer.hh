#ifndef __SPYC_CODECONSUMER_HH__
#define __SPYC_CODECONSUMER_HH__

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>

#include "CodeModel.hh"
#include "CodeVisitor.hh"

namespace spyc {

    class CodeConsumer : public clang::ASTConsumer {
    public:
        explicit CodeConsumer(clang::ASTContext* astContext, CodeModel& model)
            : _visitor(astContext, model)
        {
        }

        virtual void
        HandleTranslationUnit(clang::ASTContext& Context)
        {
            _visitor.TraverseDecl(Context.getTranslationUnitDecl());
        }

    private:
        CodeVisitor _visitor;
    };

} // namespace spyc

#endif /* __SPYC_CODECONSUMER_HH__ */