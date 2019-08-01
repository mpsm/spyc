#include "DotOutputter.hh"

#include <iostream>

using namespace spyc;
using std::endl;

DotOutputter::DotOutputter(const CodeModel& model)
    : _model(model)
{

}

void DotOutputter::outputCallGraph(std::ostream& os)
{
    os << "digraph callgraph {" << endl;

    for (auto fp : _model.getFunctions()) {
        auto f = fp.second;
        auto fname = f->getName();

        os << "\t// " << fname << endl;
        for (auto weak_callee : f->getCallees()) {
            auto callee = weak_callee.lock();
            os << "\t" << fname << " -> " << callee->getName() << ";" << endl;
        }
    }

    os << "}" << endl;
}