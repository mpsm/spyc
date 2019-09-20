#ifndef __SPYC_ASTVISITOR_HH__
#define __SPYC_ASTVISITOR_HH__

#include "CodeModel.hh"
#include "Method.hh"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>

#include <memory>
#include <unordered_map>

namespace spyc {

    class CodeVisitor : public clang::RecursiveASTVisitor<CodeVisitor> {
    public:
        explicit CodeVisitor(clang::ASTContext* astContext, CodeModel& model);
        bool VisitCallExpr(clang::CallExpr* expr);
        bool VisitFunctionDecl(clang::FunctionDecl* fd);

    private:
        Method::ID getFuncDeclID(clang::FunctionDecl* decl);

        clang::ASTContext* ctx;
        CodeModel& _model;
        std::shared_ptr<Method> lastCaller;
    };

} // namespace spyc

#endif