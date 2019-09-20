#ifndef __SPYC_CODEMODEL_HH__
#define __SPYC_CODEMODEL_HH__

#include "Method.hh"

#include <clang/AST/Decl.h>

#include <memory>
#include <string>
#include <unordered_map>

namespace spyc {

    class CodeModel {
        struct MethodHasher;

    public:
        using funcmap = std::unordered_map<Method::ID, std::shared_ptr<Method>,
            MethodHasher>;

        const funcmap& getFunctions() const;
        std::shared_ptr<Method> getMethod(Method::ID id);

    private:
        struct MethodHasher {
            std::size_t
            operator()(const Method::ID& id) const noexcept
            {
                return std::hash<std::string>{}(id.first + ":" + id.second);
            }
        };
        funcmap functions;
    };

} // namespace spyc

#endif /* __SPYC_CODEMODEL_HH__ */