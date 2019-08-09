#include "CodeModel.hh"
#include "Method.hh"

#include <memory>
#include <string>

using namespace spyc;

const CodeModel::funcmap& CodeModel::getFunctions() const
{
    return functions;
}

std::shared_ptr<Method> CodeModel::getMethod(Method::ID id)
{
    auto fi = functions.find(id);

    if (fi == functions.end()) {
        auto newf = std::make_shared<Method>(id);
        functions.insert({id, newf});
        return newf;
    }

    return fi->second;
}
