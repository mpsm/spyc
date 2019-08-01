#ifndef __SPYC_DOTOUTPUTTER_H__
#define __SPYC_DOTOUTPUTTER_H__

#include "CodeModel.hh"

#include <iostream>

namespace spyc {

    class DotOutputter {
    public:
        DotOutputter(const CodeModel& model);
        void outputCallGraph(std::ostream& os);

    private:
        const CodeModel &_model;
    };

}

#endif /* __SPYC_DOTOUTPUTTER_H__ */
