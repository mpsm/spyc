#ifndef __SPYC_METHOD_HH__
#define __SPYC_METHOD_HH__

#include <list>
#include <memory>
#include <string>

namespace spyc {
    
    class Method {    
    public:
        using methodlist = std::list<std::weak_ptr<Method>>;
    
        Method(const std::string& name);
        Method(void) = delete;

        const std::string& getName() const;
        const methodlist& getCallers() const;
        const methodlist& getCallees() const;

        bool operator==(const Method& method) const;
        bool operator!=(const Method& method) const;

        friend void linkMethods(std::shared_ptr<Method> caller, std::shared_ptr<Method> callee);

    private:
        void addCaller(std::shared_ptr<Method> m);
        void addCallee(std::shared_ptr<Method> m);
        void insertUniqueMethod(methodlist& mlist, std::shared_ptr<Method> m);

        methodlist callers;
        methodlist callees;
        std::string _name;
    };

    void linkMethods(std::shared_ptr<Method> caller, std::shared_ptr<Method> callee);

}

#endif /* __SPYC_METHOD_HH__ */