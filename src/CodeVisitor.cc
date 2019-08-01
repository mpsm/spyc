#include "CodeVisitor.hh"
#include "CodeModel.hh"
#include "Method.hh"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>

using namespace spyc;

CodeVisitor::CodeVisitor(clang::ASTContext *astContext, CodeModel& model)
    : ctx(astContext), _model(model)
{

}

bool CodeVisitor::VisitCallExpr(clang::CallExpr *expr)
{
    clang::FunctionDecl *callee = expr->getDirectCallee();

    if (callee != nullptr) {
        auto f = _model.createFunction(callee->getNameAsString());
        linkMethods(lastCaller, f);
    }

    return true;
}

bool CodeVisitor::VisitFunctionDecl(clang::FunctionDecl *fd)
{
    auto f = _model.createFunction(fd->getNameAsString());
    lastCaller = f;

    return true;
}
