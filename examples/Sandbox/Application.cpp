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
        ace::Logger::RegisterSink(
            std::make_shared<ace::LoggerConsoleSink>()
        );

        auto lSubscriptionID = ace::EventBus::Subscribe<ace::FileChangedEvent>(
            [] (const ace::FileChangedEvent& pEvent) -> bool
            {
                switch (pEvent.mMethod)
                {
                    case ace::FileChangeMethod::Created:
                        ACE_LOG_INFO("Created file '{}'.", pEvent.mPath);
                        break;
                    case ace::FileChangeMethod::Updated:
                        ACE_LOG_INFO("Updated file '{}'.", pEvent.mPath);
                        break;
                    case ace::FileChangeMethod::Deleted:
                        ACE_LOG_INFO("Deleted file '{}'.", pEvent.mPath);
                        break;
                }

                return true;
            }
        );

        ace::FileWatcher lWatcher;
        lWatcher.Start({ "./notes" });

        for (int i = 0; i < 30; ++i)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            ace::EventBus::Dispatch();
        }

        lWatcher.Stop();
    }

    Application::~Application ()
    {
        
    }

}
