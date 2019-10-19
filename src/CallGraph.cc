#include "CallGraph.hh"
#include "Method.hh"

#include <functional>
#include <stdexcept>
#include <utility>

using namespace spyc;

CallGraph::CallGraph(const Method& root) : _root(root)
{
    loadNode(this->_root);
}

const Method&
CallGraph::getRoot() const noexcept
{
    return this->_root;
}

const CallGraph::methodsList&
CallGraph::getMethods() const noexcept
{
    return this->_methods;
}

const CallGraph::callsList&
CallGraph::getCalls() const noexcept
{
    return this->_calls;
}

std::size_t
CallGraph::getMethodCount() const noexcept
{
    return this->_methodCount;
}

std::size_t
CallGraph::getCallCount() const noexcept
{
    return this->_callCount;
}

bool
CallGraph::hasMethod(const Method& m) const noexcept
{
    // TODO: use C++20's `contains` method when it is available
    return this->_methods.find(std::cref(m)) != this->_methods.end();
}

bool
CallGraph::hasCall(const Method& caller, const Method& callee) const noexcept
{
    return hasCall(makeCall(caller, callee));
}

bool
CallGraph::hasCall(const call& c) const noexcept
{
    return this->_calls.find(c) != this->_calls.end();
}

CallGraph::call
CallGraph::makeCall(const Method& caller, const Method& callee) const noexcept
{
    return std::make_pair(std::cref(caller), std::cref(callee));
}

void
CallGraph::loadNode(const Method& m)
{
    if (addNode(m)) {
        for (auto callee : m.getCallees()) {
            addCall(m, callee);
            loadNode(callee);
        }
    }
}

bool
CallGraph::addNode(const Method& m)
{
    if (hasMethod(m)) {
        return false;
    }

    this->_methods.insert(std::ref(m));
    this->_methodCount++;

    return true;
}

void
CallGraph::addCall(const Method& caller, const Method& callee)
{
    auto call = makeCall(caller, callee);

    if (hasCall(call)) {
        throw std::logic_error("Call has been already added.");
    }

    this->_calls.insert(makeCall(caller, callee));
    this->_callCount++;
}