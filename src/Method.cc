#include "Method.hh"

#include <memory>
#include <string>

using std::string;
using namespace spyc;

Method::Method(const string& name) : _name(name)
{

}

const string& Method::getName() const
{
    return _name;
}

bool Method::operator==(const Method& method)
{
    return this->_name == method._name;
}

const Method::methodlist& Method::getCallers() const
{
    return callers;
}

const Method::methodlist& Method::getCallees() const
{
    return callees;
}

void Method::addCaller(std::shared_ptr<Method> m)
{
    callers.push_back(m);
}

void Method::addCallee(std::shared_ptr<Method>  m)
{
    callees.push_back(m);
}

void spyc::linkMethods(std::shared_ptr<Method> caller, std::shared_ptr<Method> callee)
{
    caller->addCallee(callee);
    callee->addCaller(caller);
}