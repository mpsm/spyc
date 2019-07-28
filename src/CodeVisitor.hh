#ifndef __SPYC_ASTVISITOR_HH__
#define __SPYC_ASTVISITOR_HH__

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>

namespace spyc {

    class CodeVisitor : public clang::RecursiveASTVisitor<CodeVisitor> {
    public:
        explicit CodeVisitor(clang::ASTContext *astContext);
        bool VisitCallExpr(clang::CallExpr *expr);
        bool VisitFunctionDecl(clang::FunctionDecl *fd);

    private:
        clang::ASTContext *ctx;
    };

}

#endif