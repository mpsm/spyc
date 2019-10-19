#ifndef __SPYC_CALLGRAPH_HH__
#define __SPYC_CALLGRAPH_HH__

#include "Method.hh"

#include <functional>
#include <unordered_set>

namespace spyc {
    class CallGraph {
    public:
        using methodsList =
            std::unordered_set<std::reference_wrapper<const Method>,
                std::hash<Method>, std::equal_to<Method>>;

        using call = std::pair<std::reference_wrapper<const Method>,
            std::reference_wrapper<const Method>>;

    private:
        struct CallHasher {
            std::size_t
            operator()(const call& c) const noexcept
            {
                return ~std::hash<Method>{}(c.first.get()) |
                       std::hash<Method>{}(c.second.get());
            }
        };

        struct CallCompare {
            bool
            operator()(const call& left, const call& right) const noexcept
            {
                return (left.first.get() == right.first.get()) &&
                       (left.second.get() == right.second.get());
            }
        };

    public:
        using callsList = std::unordered_set<call, CallHasher, CallCompare>;

        CallGraph() = delete;
        explicit CallGraph(const Method& root);

        const Method& getRoot() const noexcept;
        const methodsList& getMethods() const noexcept;
        const callsList& getCalls() const noexcept;
        std::size_t getMethodCount() const noexcept;
        std::size_t getCallCount() const noexcept;
        bool hasMethod(const Method& m) const noexcept;
        bool hasCall(const call& c) const noexcept;
        bool hasCall(const Method& caller, const Method& callee) const noexcept;

    private:
        bool addNode(const Method& m);
        void addCall(const Method& caller, const Method& callee);
        void loadNode(const Method& m);
        call makeCall(const Method& caller, const Method& callee) const
            noexcept;

        const Method& _root;
        methodsList _methods;
        callsList _calls;
        std::size_t _methodCount{0};
        std::size_t _callCount{0};
    };
} // namespace spyc

#endif