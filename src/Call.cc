#include "Call.hh"
#include "Method.hh"

#include <functional>
#include <utility>

spyc::Call
spyc::make_call(const spyc::Method& caller, const spyc::Method& callee)
{
    return std::make_pair<std::reference_wrapper<const spyc::Method>,
        std::reference_wrapper<const spyc::Method>>(
        std::cref(caller), std::cref(callee));
}
