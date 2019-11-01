#ifndef __SPYC_CALL_HH__
#define __SPYC_CALL_HH__

#include "Method.hh"

#include <functional>
#include <unordered_set>
#include <utility>

namespace spyc {
    using Call = std::pair<std::reference_wrapper<const Method>,
        std::reference_wrapper<const Method>>;

    struct CallHasher {
        std::size_t
        operator()(const Call& c) const noexcept
        {
            return ~std::hash<Method>{}(c.first.get()) |
                   std::hash<Method>{}(c.second.get());
        }
    };

    struct CallCompare {
        bool
        operator()(const Call& left, const Call& right) const noexcept
        {
            return (left.first.get() == right.first.get()) &&
                   (left.second.get() == right.second.get());
        }
    };

    using CallSet = std::unordered_set<Call, CallHasher, CallCompare>;

    Call make_call(const Method& caller, const Method& callee);

} // namespace spyc

#endif /* __SPYC_CALL_HH__ */