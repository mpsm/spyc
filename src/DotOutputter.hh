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

            for (auto call : range) {
                const auto& [caller, callee] = call;
                os << "\t" << caller.get().getName() << " -> "
                   << callee.get().getName() << ";" << std::endl;
            }

            os << "}" << std::endl;
        }

    private:
        std::ostream& os;
    };

} // namespace spyc

#endif /* __SPYC_DOTOUTPUTTER_H__ */
