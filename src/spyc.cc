#include "CallGraph.hh"
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

enum class CallDiagramType { CALL_MAP, CALL_GRAPH };
llvm::cl::opt<enum CallDiagramType> CallDiagramTypeOption{"diagram-type",
    llvm::cl::cat(category), llvm::cl::desc("Call diagram type"),
    llvm::cl::values(clEnumValN(CallDiagramType::CALL_MAP, "map", "Map"),
        clEnumValN(CallDiagramType::CALL_GRAPH, "graph", "Graph")),
    llvm::cl::init(CallDiagramType::CALL_MAP)};

llvm::cl::opt<std::string> CallGraphStartOption{"graph-start",
    llvm::cl::cat(category), llvm::cl::desc("Call graph starting point"),
    llvm::cl::init("main")};

static void
printVersion(llvm::raw_ostream& os)
{
    static const std::string version = "0.1";
    os << "  SpyC version: " << version;
    if (spyc::gitversion != nullptr) {
        os << "." << spyc::gitversion;
    }
    os << "\n";
}

int
main(int argc, const char** argv)
{
    int err;
    llvm::llvm_shutdown_obj llvmCleanup;

    spyc::CodeModel model;

    llvm::cl::AddExtraVersionPrinter(printVersion);
    clang::tooling::CommonOptionsParser optionParser(argc, argv, category);
    clang::tooling::ClangTool tool(
        optionParser.getCompilations(), optionParser.getSourcePathList());

    err = tool.run(
        std::make_unique<spyc::SpycFrontendActionFactory>(model).get());

    if (err != 0) {
        return err;
    }

    spyc::DotOutputter outputter{std::cout};

    switch (CallDiagramTypeOption.getValue()) {
    case CallDiagramType::CALL_MAP:
        outputter.outputCallGraph(model.getCalls());
        break;

    case CallDiagramType::CALL_GRAPH:
        const auto& startPoint = CallGraphStartOption.getValue();
        auto found = model.findMethodsByName(startPoint);
        if (found.size() == 0) {
            std::cerr << "Cannot find method: " << startPoint << std::endl;
            return 1;
        } else if (found.size() > 1) {
            std::cerr << "Multiple methods found: " << startPoint << std::endl;
            return 1;
        }

        spyc::CallGraph cg(*found[0]);
        outputter.outputCallGraph(cg.getCalls());
        break;
    }

    return 0;
}