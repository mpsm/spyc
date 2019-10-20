#ifndef __SPYC_METHOD_HH__
#define __SPYC_METHOD_HH__

#include <functional>
#include <list>
#include <memory>
#include <string>

namespace spyc {

    class Method {
    public:
        using methodlist = std::list<std::reference_wrapper<Method>>;
        using ID = std::pair<std::string, std::string>;

        explicit Method(const std::string& name);
        explicit Method(const ID& methodId);
        Method(void) = delete;

        const std::string& getName() const;
        const std::string& getFileName() const;
        const ID& getID() const;
        const methodlist& getCallers() const;
        const methodlist& getCallees() const;

        bool operator==(const Method& method) const;
        bool operator!=(const Method& method) const;

        friend void linkMethods(Method& caller, Method& callee);

    private:
        void addCaller(Method& m);
        void addCallee(Method& m);
        void insertUniqueMethod(methodlist& mlist, Method& m);

        methodlist callers;
        methodlist callees;
        ID id;
    };

    void linkMethods(Method& caller, Method& callee);

} // namespace spyc

#endif /* __SPYC_METHOD_HH__ */