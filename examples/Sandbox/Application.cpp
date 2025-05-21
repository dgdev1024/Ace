/**
 * @file    Sandbox/Application.cpp
 */

#include <iostream>
#include <Sandbox/Application.hpp>

std::unique_ptr<ace::IApplication> ace::MakeApplication ()
{
    ace::ApplicationSpec pSpec;
    return std::make_unique<sandbox::Application>(pSpec);
}

namespace sandbox
{

    /* Constructors and Destructor ********************************************/

    Application::Application (
        const ace::ApplicationSpec& pSpec
    ):
        ace::IApplication   { pSpec }
    {
        ace::VFS::MountPhysicalDirectory("my_notes", "./notes");
        if (auto lFile = ace::VFS::OpenLogicalFile("my_notes/cgpt.assets.md"))
        {
            astd::byte_buffer lBuffer(lFile->GetSize());
            lFile->Read(lBuffer.data(), lBuffer.size());
            for (const auto& lByte : lBuffer)
            {
                std::printf("%c", lByte);
            }

            std::printf("\n");
        }
    }

    Application::~Application ()
    {
        
    }

}
