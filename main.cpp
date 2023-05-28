#include <iostream>
#include <thread>

#include "nador/App.h"
#include "nador/log/StandardLogger.h"
#include "nador/log/StreamLogger.h"

enum Fonts : uint32_t
{
    FREE_SANS = 1,
};

enum FontSize : uint32_t
{
    SMALL  = 32,
    MEDIUM = 64,
    LARGE  = 128,
};

enum Sound : uint32_t
{
    TEST_SOUND_1,
    TEST_SOUND_2,
};

void InitFonts()
{
    auto start = std::chrono::system_clock::now();

    nador::IFontController* fontCtrl = nador::IApp::Get()->GetFontController();

    fontCtrl->AddFontSize(FontSize::SMALL);
    fontCtrl->AddFontSize(FontSize::MEDIUM);
    fontCtrl->AddFontSize(FontSize::LARGE);

    fontCtrl->RegisterFont(Fonts::FREE_SANS, "res/fonts/FreeSans.ttf");

    // Sets the default font size
    fontCtrl->SetDefaultSystemFont(Fonts::FREE_SANS, FontSize::SMALL);

    while (fontCtrl->IsLoading())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    auto diff = std::chrono::system_clock::now() - start;
    NADOR_DEBUG("InitFonts duration: %d ms", std::chrono::duration_cast<std::chrono::milliseconds>(diff));
}

void InitSounds()
{
    nador::ISoundController* soundCtrl = nador::IApp::Get()->GetSoundController();

    soundCtrl->AddSound("res/sounds/TestSound.wav", Sound::TEST_SOUND_1);
    soundCtrl->AddSound("res/sounds/TestSound_Mono.wav", Sound::TEST_SOUND_2);
}

int main(void)
{
    nador::StandardLogger standardLogger;
    nador::StreamLogger<std::chrono::system_clock>   engineStreamLogger(nador::GetStream<std::ofstream>("logs/nador_engine.log"), std::chrono::milliseconds(100));
    nador::StreamLogger<std::chrono::system_clock>   userstreamLogger(nador::GetStream<std::ofstream>("logs/nador.log"), std::chrono::seconds(2));

    // setup default standard logging
    NADOR_LOG.RegisterCallback(nador::ELogType::ENGINE_DEBUG, [standardLogger, engineStreamLogger](std::string_view msg) mutable {
        standardLogger.Debug(msg);
        engineStreamLogger.Log(msg);
    });
    NADOR_LOG.RegisterCallback(nador::ELogType::ENGINE_WARNING, [standardLogger, engineStreamLogger](std::string_view msg) mutable {
        standardLogger.Warning(msg);
        engineStreamLogger.Log(msg);
    });
    NADOR_LOG.RegisterCallback(nador::ELogType::ENGINE_ERROR, [standardLogger, engineStreamLogger](std::string_view msg) mutable {
        standardLogger.Error(msg);
        engineStreamLogger.Log(msg);
    });
    NADOR_LOG.RegisterCallback(nador::ELogType::ENGINE_FATAL, [standardLogger, engineStreamLogger](std::string_view msg) mutable {
        standardLogger.Fatal(msg);
        engineStreamLogger.Log(msg);
    });

    NADOR_LOG.RegisterCallback(nador::ELogType::DEBUG, [standardLogger, userstreamLogger](std::string_view msg) mutable {
        standardLogger.Debug(msg);
        userstreamLogger.Log(msg);
    });
    NADOR_LOG.RegisterCallback(nador::ELogType::WARNING, [standardLogger, userstreamLogger](std::string_view msg) mutable {
        standardLogger.Warning(msg);
        userstreamLogger.Log(msg);
    });
    NADOR_LOG.RegisterCallback(nador::ELogType::ERROR, [standardLogger, userstreamLogger](std::string_view msg) mutable {
        standardLogger.Error(msg);
        userstreamLogger.Log(msg);
    });
    NADOR_LOG.RegisterCallback(nador::ELogType::FATAL, [standardLogger, userstreamLogger](std::string_view msg) mutable {
        standardLogger.Fatal(msg);
        userstreamLogger.Log(msg);
    });

    // Initialize the app with the config settings.
    nador::AppConfig config = nador::ReadAppConfigFromFile("app_config.xml");

    // Create the main Application.
    nador::IAppUPtr app = nador::App::CreateApp(config);

    // Adding default tests
    app->InitializeDefaultTests();

    InitFonts();
    InitSounds();

    // Game loop
    app->Run();

    return 0;
}