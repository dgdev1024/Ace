/**
 * @file    AceBox/Application.hpp
 */

#pragma once
#include <Ace.hpp>

namespace acebox
{

    class Application final : public ace::Application
    {
    public:

        explicit Application (
            const ace::ApplicationSpecification& pSpec
        );

        virtual ~Application () override;

    private:

        std::uint32_t mTheAnswer = 42;

    };

}
