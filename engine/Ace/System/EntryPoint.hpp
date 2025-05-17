/**
 * @file    Ace/System/EntryPoint.hpp
 * @brief   Provides an entry point for the client application.
 */

#pragma once
#include <Ace/System/IApplication.hpp>

#if !defined(ACE_USE_OWN_MAIN)
#if defined(ACE_LINUX)

int main ()
{
    auto lApp = ace::MakeApplication();
    return lApp->Start();
}

#else
    #error "The Ace Engine does not currently support your operating system."
#endif
#endif
