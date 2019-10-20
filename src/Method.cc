#include "Method.hh"

#include <algorithm>
#include <functional>
#include <memory>
#include <string>

using std::string;
using namespace spyc;

Method::Method(const string& name) : Method(Method::ID(name, "")) {}

Method::Method(const ID& methodId) : id{methodId} {}

const string&
Method::getName() const
{
    return id.first;
}

const std::string&
Method::getFileName() const
{
    return id.second;
}

const Method::ID&
Method::getID() const
{
    return this->id;
}

bool
Method::operator==(const Method& method) const
{
    return this->id == method.id;
}

bool
Method::operator!=(const Method& method) const
{
    return !(this->operator==(method));
}

const Method::methodlist&
Method::getCallers() const
{
    return callers;
}

const Method::methodlist&
Method::getCallees() const
{
    return callees;
}

void
Method::addCaller(Method& m)
{
    insertUniqueMethod(callers, m);
}

void
Method::addCallee(Method& m)
{
    insertUniqueMethod(callees, m);
}

void
Method::insertUniqueMethod(methodlist& mlist, Method& m)
{
    if (std::find_if(std::begin(callees), std::end(callees),
            [&m](std::reference_wrapper<Method> p) { return m == p.get(); }) ==
        std::end(callees)) {
        mlist.push_back(m);
    }
}

void
spyc::linkMethods(Method& caller, Method& callee)
{
    caller.addCallee(callee);
    callee.addCaller(caller);
}