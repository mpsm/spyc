#include "CodeConsumer.hh"
#include "CodeModel.hh"
#include "DotOutputter.hh"
#include "SpycFrontendFactory.hh"

#include "gitversion.hh"

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <string>

static llvm::cl::OptionCategory category("SpyC");

static void printVersion(llvm::raw_ostream &os)
{
    static const std::string version = "0.0.1";
    os << "  SpyC version: " << version;
    if (spyc::gitversion != nullptr) {
        os << "." << spyc::gitversion;
    }
    os << "\n";
}

int main(int argc, const char **argv)
{
    int err;
    llvm::llvm_shutdown_obj llvmCleanup;

    spyc::CodeModel model;

    llvm::cl::AddExtraVersionPrinter(printVersion);
    clang::tooling::CommonOptionsParser optionParser(argc, argv, category);
    clang::tooling::ClangTool tool(optionParser.getCompilations(), optionParser.getSourcePathList());

    err = tool.run(std::make_unique<spyc::SpycFrontendActionFactory>(model).get());

    if (err != 0) {
        return err;
    }

    {
        spyc::DotOutputter outputter{std::cout};
        outputter.outputCallGraph(std::cout, model.getFunctions());
    }

    return 0;
}