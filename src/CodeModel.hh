#ifndef __SPYC_CODEMODEL_HH__
#define __SPYC_CODEMODEL_HH__

#include "Method.hh"

#include <memory>
#include <string>
#include <unordered_map>

namespace spyc {

    class CodeModel {
    public:
        using funcmap = std::unordered_map<std::string, std::shared_ptr<Method>>;

        const funcmap& getFunctions() const;
        std::shared_ptr<Method> getFunctionByName(const std::string& name);
        std::shared_ptr<Method> createFunction(const std::string& name);

    private:
        funcmap functions;
    };

}

#endif /* __SPYC_CODEMODEL_HH__ */