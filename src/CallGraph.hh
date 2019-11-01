#ifndef __SPYC_CALLGRAPH_HH__
#define __SPYC_CALLGRAPH_HH__

#include "Call.hh"
#include "Method.hh"

#include <functional>
#include <unordered_set>

namespace spyc {
    class CallGraph {
    public:
        using methodsList =
            std::unordered_set<std::reference_wrapper<const Method>,
                std::hash<Method>, std::equal_to<Method>>;

    public:
        CallGraph() = delete;
        explicit CallGraph(const Method& root);

        const Method& getRoot() const noexcept;
        const methodsList& getMethods() const noexcept;
        const CallSet& getCalls() const noexcept;
        std::size_t getMethodCount() const noexcept;
        std::size_t getCallCount() const noexcept;
        bool hasMethod(const Method& m) const noexcept;
        bool hasCall(const Call& c) const noexcept;
        bool hasCall(const Method& caller, const Method& callee) const noexcept;

    private:
        bool addNode(const Method& m);
        void addCall(const Method& caller, const Method& callee);
        void loadNode(const Method& m);

        const Method& _root;
        methodsList _methods;
        CallSet _calls;
        std::size_t _methodCount{0};
        std::size_t _callCount{0};
    };
} // namespace spyc

#endif