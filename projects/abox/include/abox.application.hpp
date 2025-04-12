/**
 * @file  abox.application.hpp
 */

#pragma once
#include <ace.hpp>

namespace abox
{

    class application final : public ace::application_base
    {
    public:
        application (const ace::application_specification& p_spec);
        ~application () override;

    };

}
