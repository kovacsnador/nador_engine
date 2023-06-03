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
    TEST_SOUND_1 = 1,
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

    soundCtrl->LoadSound("res/sounds/TestSound.wav", Sound::TEST_SOUND_1);
    soundCtrl->LoadSound("res/sounds/TestSound_Mono.wav", Sound::TEST_SOUND_2);
}

void SetupLogging()
{
    auto log = std::make_unique<nador::Log<>>();

    nador::StandardLogger standardLogger;
    nador::StreamLogger   engineStreamLogger(nador::GetStream<std::ofstream>("logs/nador_engine.log"));
    nador::StreamLogger   userstreamLogger(nador::GetStream<std::ofstream>("logs/nador.log"));

    // setup default standard logging
    log->RegisterCallback(nador::ELogType::ENGINE_DEBUG, [standardLogger, engineStreamLogger](std::string_view msg) mutable {
        standardLogger.Debug(msg);
        engineStreamLogger.Write(msg);
    });
    log->RegisterCallback(nador::ELogType::ENGINE_WARNING, [standardLogger, engineStreamLogger](std::string_view msg) mutable {
        standardLogger.Warning(msg);
        engineStreamLogger.Write(msg);
    });
    log->RegisterCallback(nador::ELogType::ENGINE_ERROR, [standardLogger, engineStreamLogger](std::string_view msg) mutable {
        standardLogger.Error(msg);
        engineStreamLogger.Write(msg);
    });
    log->RegisterCallback(nador::ELogType::ENGINE_FATAL, [standardLogger, engineStreamLogger](std::string_view msg) mutable {
        standardLogger.Fatal(msg);
        engineStreamLogger.Write(msg);
        throw std::runtime_error(msg.data());
    });

    log->RegisterCallback(nador::ELogType::DEBUG, [standardLogger, userstreamLogger](std::string_view msg) mutable {
        standardLogger.Debug(msg);
        userstreamLogger.Write(msg);
    });
    log->RegisterCallback(nador::ELogType::WARNING, [standardLogger, userstreamLogger](std::string_view msg) mutable {
        standardLogger.Warning(msg);
        userstreamLogger.Write(msg);
    });
    log->RegisterCallback(nador::ELogType::ERROR, [standardLogger, userstreamLogger](std::string_view msg) mutable {
        standardLogger.Error(msg);
        userstreamLogger.Write(msg);
    });
    log->RegisterCallback(nador::ELogType::FATAL, [standardLogger, userstreamLogger](std::string_view msg) mutable {
        standardLogger.Fatal(msg);
        userstreamLogger.Write(msg);
        throw std::runtime_error(msg.data());
    });

    nador::SetLoggingInterface(std::move(log));
}

int main(void)
{
    // Logger setup
    SetupLogging();

    {
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
    }

    return 0;
}