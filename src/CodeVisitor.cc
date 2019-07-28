#include "CodeVisitor.hh"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>

#include <iostream>

using namespace spyc;

CodeVisitor::CodeVisitor(clang::ASTContext *astContext) : ctx(astContext)
{

}

bool CodeVisitor::VisitCallExpr(clang::CallExpr *expr)
{
    clang::FunctionDecl *callee = expr->getDirectCallee();

    if (callee != nullptr) {
        std::cout << "    " << callee->getNameAsString();
        std::cout << " -> " << expr->getCallReturnType(*ctx).getAsString() << std::endl;
    }

    return true;
}

bool CodeVisitor::VisitFunctionDecl(clang::FunctionDecl *fd)
{
    std::cout << "Function declared: " << fd->getNameAsString() << std::endl;
    return true;
}