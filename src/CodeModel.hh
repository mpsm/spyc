#ifndef __SPYC_CODEMODEL_HH__
#define __SPYC_CODEMODEL_HH__

#include "Method.hh"

#include <memory>
#include <string>
#include <unordered_map>

namespace spyc {

    class CodeModelInterface {
    public:
        virtual std::shared_ptr<Method> getMethod(Method::ID) = 0;
        virtual ~CodeModelInterface() {}
    };

    class CodeModel : public CodeModelInterface {
        struct MethodHasher;

    public:
        virtual ~CodeModel() = default;

        using funcmap = std::unordered_map<Method::ID, std::shared_ptr<Method>>;

        const funcmap& getFunctions() const;
        std::shared_ptr<Method> getMethod(Method::ID id);

    private:
        funcmap functions;
    };

} // namespace spyc

#endif /* __SPYC_CODEMODEL_HH__ */