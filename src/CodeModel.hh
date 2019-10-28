#ifndef __SPYC_CODEMODEL_HH__
#define __SPYC_CODEMODEL_HH__

#include "Call.hh"
#include "Method.hh"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace spyc {

    class CodeModelInterface {
    public:
        virtual std::shared_ptr<Method> getMethod(Method::ID) = 0;
        virtual void addCall(Method& caller, Method& callee) = 0;
        virtual ~CodeModelInterface() {}
    };

    class CodeModel : public CodeModelInterface {
    public:
        using funcmap = std::unordered_map<Method::ID, std::shared_ptr<Method>>;

        virtual ~CodeModel() = default;

        const funcmap& getFunctions() const;
        const CallSet& getCalls() const;

        std::vector<std::shared_ptr<Method>> findMethodsByName(
            const std::string& name) const;

        std::shared_ptr<Method> getMethod(Method::ID id);
        void addCall(Method& caller, Method& callee);

    private:
        bool hasMethod(const Method& m);

        funcmap functions;
        CallSet calls;
    };

} // namespace spyc

#endif /* __SPYC_CODEMODEL_HH__ */