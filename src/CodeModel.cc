#include "CodeModel.hh"
#include "Method.hh"

#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <utility>

using namespace spyc;

const CodeModel::funcmap&
CodeModel::getFunctions() const
{
    return functions;
}

std::shared_ptr<Method>
CodeModel::getMethod(Method::ID id)
{
    auto fi = functions.find(id);

    if (fi == functions.end()) {
        auto newf = std::make_shared<Method>(id);
        functions.insert({id, newf});
        return newf;
    }

    return fi->second;
}

void
CodeModel::addCall(Method& caller, Method& callee)
{
    if (!hasMethod(caller) || !hasMethod(callee)) {
        throw(std::runtime_error(
            "Both caller and callee must be created by model"));
    }

    calls.emplace(caller, callee);
    linkMethods(caller, callee);
}

const CallSet&
CodeModel::getCalls() const
{
    return this->calls;
}

bool
CodeModel::hasMethod(const Method& m)
{
    return functions.find(m.getID()) != functions.end();
}