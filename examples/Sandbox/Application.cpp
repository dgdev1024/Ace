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

    class TextLoader final : public ace::IAssetLoader<std::string>
    {
    public:

        bool CanLoad (
            const std::string&,
            const ace::IVirtualFile&
        ) const override
        {
            return true;
        }

        std::shared_ptr<std::string> Load (
            std::unique_ptr<ace::IVirtualFile> pAssetFile
        ) override
        {
            auto lString = std::make_shared<std::string>();
            lString->resize(pAssetFile->GetSize());

            pAssetFile->Read(lString->data());
            return lString;
        }

    };

    /* Constructors and Destructor ********************************************/

    Application::Application (
        const ace::ApplicationSpec& pSpec
    ):
        ace::IApplication   { pSpec }
    {
        ace::VFS::MountArchive("my_test", "./notes/test.zip");
        ace::AssetRegistry::RegisterAssetLoader<std::string>(
            std::make_shared<TextLoader>()
        );
        if (
            auto lAsset =
                ace::AssetRegistry::Load<std::string>("my_test/cgpt.roadmap.md")
        )
        {
            std::printf("%s\n", lAsset->c_str());
        }

        // if (auto lFile = ace::VFS::OpenFile("my_test/cgpt.roadmap.md"))
        // {
        //     astd::byte_buffer lBuffer(lFile->GetSize());
        //     lFile->Read(lBuffer.data(), lBuffer.size());
        //     for (const auto& lByte : lBuffer)
        //     {
        //         std::printf("%c", lByte);
        //     }

        //     std::printf("\n");
        // }
    }

    Application::~Application ()
    {
        
    }

}
