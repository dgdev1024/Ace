/**
 * @file    Ace/System/EntryPoint.hpp
 * @brief   Provides an entry point to be used by the client application.
 */

#pragma once
#include <Ace/System/Application.hpp>

#if defined(ACE_LINUX)

int main (int argc, char** argv)
{
    ACE_UNUSED(argc);
    ACE_UNUSED(argv);

    auto app = ace::makeApplication();
    return app->start();
}

#endif
