#ifndef __SPYC_DOTOUTPUTTER_H__
#define __SPYC_DOTOUTPUTTER_H__

#include <iostream>
namespace spyc {

    class DotOutputter {
    public:
        DotOutputter() = delete;
        explicit DotOutputter(std::ostream& outputStream) : os(outputStream){};

        template <typename T>
        void
        outputCallGraph(const T& range) const
        {
            os << "digraph callgraph {" << std::endl;

            for (auto fp : range) {
                auto f = fp.second;
                auto fname = f->getName();

                os << "\t// " << fname << std::endl;
                for (auto weak_callee : f->getCallees()) {
                    auto callee = weak_callee.lock();
                    os << "\t" << fname << " -> " << callee->getName() << ";"
                       << std::endl;
                }
            }

            os << "}" << std::endl;
        }

    private:
        std::ostream& os;
    };

} // namespace spyc

#endif /* __SPYC_DOTOUTPUTTER_H__ */
