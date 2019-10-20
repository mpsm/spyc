#include "CodeVisitor.hh"
#include "CodeModel.hh"
#include "Method.hh"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>

#include <utility>

using namespace spyc;

CodeVisitor::CodeVisitor(
    clang::ASTContext* astContext, CodeModelInterface& model)
    : ctx(astContext), _model(model)
{
}

bool
CodeVisitor::VisitCallExpr(clang::CallExpr* expr)
{
    clang::FunctionDecl* callee = expr->getDirectCallee();

    if (callee != nullptr) {
        auto f = _model.getMethod(getFuncDeclID(callee));
        linkMethods(*lastCaller, *f);
    }

    return true;
}

bool
CodeVisitor::VisitFunctionDecl(clang::FunctionDecl* fd)
{
    if (fd->hasBody()) {
        lastCaller = _model.getMethod(getFuncDeclID(fd));
    }

    return true;
}

Method::ID
CodeVisitor::getFuncDeclID(clang::FunctionDecl* decl)
{
    auto& srcmng = ctx->getSourceManager();
    auto filename = srcmng.getFilename(decl->getBeginLoc()).str();
    auto name = decl->getNameAsString();

    return Method::ID{name, filename};
}
