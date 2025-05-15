/**
 * @file    Ace/System/EntryPoint.hpp
 * @brief   Provides an entry point for the client application.
 */

#pragma once
#include <Ace/System/Application.hpp>

#if !defined(ACE_USE_OWN_MAIN)
#if defined(ACE_LINUX)

int main ()
{
    auto lApp = ace::makeApplication();
    return lApp->start();
}

#else
    #error "The Ace Engine does not currently support your operating system."
#endif
#endif
