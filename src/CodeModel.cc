#include "CodeModel.hh"
#include "Method.hh"

#include <memory>
#include <string>

using namespace spyc;

const CodeModel::funcmap& CodeModel::getFunctions() const
{
    return functions;
}

std::shared_ptr<Method> CodeModel::createFunction(const std::string& name)
{
    auto f = getFunctionByName(name);

    if (f == nullptr) {
        f = std::make_shared<Method>(name);
        functions.insert({name, f});
    }

    return f;
}

std::shared_ptr<Method> CodeModel::getFunctionByName(const std::string& name)
{
    auto f = functions.find(name);
    if (f == functions.end()) {
        return nullptr;
    }
    else {
        return f->second;
    }
}