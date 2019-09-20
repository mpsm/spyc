#ifndef __SPYC_SPYCFRONTENDFACTORY_HH__
#define __SPYC_SPYCFRONTENDFACTORY_HH__

#include "CodeModel.hh"
#include "SpycFrontendAction.hh"

#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>

namespace spyc {

    class SpycFrontendActionFactory
        : public clang::tooling::FrontendActionFactory {
    public:
        explicit SpycFrontendActionFactory(spyc::CodeModel& model)
            : _model(model)
        {
        }

        clang::FrontendAction*
        create() override
        {
            return new SpycFrontendAction(_model);
        }

    private:
        spyc::CodeModel& _model;
    };

} // namespace spyc

#endif /* __SPYC_SPYCFRONTENDFACTORY_HH__ */
