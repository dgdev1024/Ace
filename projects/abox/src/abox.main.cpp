/**
 * @file  abox.main.cpp
 */

#include <abox.application.hpp>

int main (int argc, char** argv)
{
    ace::application_specification l_spec;
    abox::application l_app { l_spec };
    return l_app.start();
}