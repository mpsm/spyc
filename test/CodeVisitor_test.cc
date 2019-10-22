#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "CodeModel.hh"
#include "CodeVisitor.hh"
#include "Method.hh"

#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/ADT/StringRef.h>

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

namespace fs = std::filesystem;
using ::testing::_;
using ::testing::Return;

class MockModel : public spyc::CodeModelInterface {
public:
    MOCK_METHOD(
        std::shared_ptr<spyc::Method>, getMethod, (spyc::Method::ID id));
};

class TestCodeConsumer : public clang::ASTConsumer {
public:
    explicit TestCodeConsumer(clang::ASTContext* astContext, MockModel& model)
        : _visitor(astContext, model)
    {
    }

    virtual void
    HandleTranslationUnit(clang::ASTContext& Context)
    {
        _visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    spyc::CodeVisitor _visitor;
};

class TestFrontendAction : public clang::ASTFrontendAction {
public:
    explicit TestFrontendAction(MockModel& model) : _model(model) {}

    virtual std::unique_ptr<clang::ASTConsumer>
    CreateASTConsumer(clang::CompilerInstance& Compiler, llvm::StringRef InFile)
    {
        return std::unique_ptr<clang::ASTConsumer>(
            new TestCodeConsumer(&Compiler.getASTContext(), _model));
    }

private:
    MockModel& _model;
};

class TestFrontendActionFactory : public clang::tooling::FrontendActionFactory {
public:
    explicit TestFrontendActionFactory(MockModel& model) : _model(model) {}

    clang::FrontendAction*
    create() override
    {
        return new TestFrontendAction(_model);
    }

private:
    MockModel& _model;
};

class DummyCompilationDatabase : public clang::tooling::CompilationDatabase {
private:
    fs::path toolPath;

public:
    DummyCompilationDatabase() : toolPath(fs::current_path() / "clang-tool") {}

    virtual std::vector<clang::tooling::CompileCommand>
    getCompileCommands(llvm::StringRef filePath) const override
    {
        return {{".", filePath, {toolPath.string(), filePath.str()}, ""}};
    }
};

int
run_test_tool(MockModel& model, const std::string& testName)
{
    DummyCompilationDatabase db;
    clang::tooling::ClangTool tool(db, {testName});
    auto actionFactory = std::make_unique<TestFrontendActionFactory>(model);
    return tool.run(actionFactory.get());
}

std::string
get_test_filename(const std::string& name)
{
    return fs::absolute("test/testfiles/" + name + ".c");
}

TEST(CodeVisitorTest, OneDef)
{
    MockModel model;
    std::string filename = get_test_filename("onedef");

    EXPECT_CALL(model, getMethod(spyc::Method::ID("foo", filename))).Times(1);

    ASSERT_EQ(run_test_tool(model, filename), 0);
}

TEST(CodeVisitorTest, OneCall)
{
    MockModel model;
    std::string filename = get_test_filename("onecall");
    auto foo =
        std::make_shared<spyc::Method>(spyc::Method::ID{"foo", filename});
    auto bar =
        std::make_shared<spyc::Method>(spyc::Method::ID{"bar", filename});

    EXPECT_CALL(model, getMethod(bar->getID())).WillOnce(Return(bar));
    EXPECT_CALL(model, getMethod(foo->getID())).WillOnce(Return(foo));

    ASSERT_EQ(run_test_tool(model, filename), 0);

    ASSERT_EQ(foo->getCallees().size(), 1U);
    ASSERT_EQ(bar->getCallers().size(), 1U);

    ASSERT_EQ(foo->getCallers().size(), 0U);
    ASSERT_EQ(bar->getCallees().size(), 0U);

    auto callee = foo->getCallees().begin()->get();
    auto caller = bar->getCallers().begin()->get();

    ASSERT_EQ(callee, *bar);
    ASSERT_EQ(caller, *foo);
}