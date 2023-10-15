#include <iostream>

#include "nador/App.h"
#include "nador/log/StandardLogger.h"
#include "nador/log/StreamLogger.h"
#include "nador/common/Stopwatch.h"

#include "nador/video/renderer/Camera.h"

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
    nador::Stopwatch sw;

    nador::IFontController* fontCtrl = nador::IApp::Get()->GetFontController();
    nador::IFileController* fileCtrl = nador::IApp::Get()->GetFileController();

    fontCtrl->AddFontSize(FontSize::SMALL);
    fontCtrl->AddFontSize(FontSize::MEDIUM);
    fontCtrl->AddFontSize(FontSize::LARGE);

    fontCtrl->CreateFont(Fonts::FREE_SANS, fileCtrl->Read("res/fonts/FreeSans.ttf"));

    // Sets the default font size
    fontCtrl->SetDefaultSystemFont(Fonts::FREE_SANS, FontSize::SMALL);

    fontCtrl->Wait();

    NADOR_DEBUG("InitFonts duration: %d ms", sw.Stop<std::chrono::milliseconds>().count());
}

void InitSounds()
{
    nador::Stopwatch sw;

    nador::ISoundController* soundCtrl = nador::IApp::Get()->GetSoundController();

    soundCtrl->LoadSound("res/sounds/TestSound.wav", Sound::TEST_SOUND_1);
    soundCtrl->LoadSound("res/sounds/TestSound_Mono.wav", Sound::TEST_SOUND_2);

    NADOR_DEBUG("InitSounds duration: %d ms", sw.Stop<std::chrono::milliseconds>().count());
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
        engineStreamLogger << msg;
    });
    log->RegisterCallback(nador::ELogType::ENGINE_WARNING, [standardLogger, engineStreamLogger](std::string_view msg) mutable {
        standardLogger.Warning(msg);
        engineStreamLogger << msg;
    });
    log->RegisterCallback(nador::ELogType::ENGINE_ERROR, [standardLogger, engineStreamLogger](std::string_view msg) mutable {
        standardLogger.Error(msg);
        engineStreamLogger << msg;
    });
    log->RegisterCallback(nador::ELogType::ENGINE_FATAL, [standardLogger, engineStreamLogger](std::string_view msg) mutable {
        standardLogger.Fatal(msg);
        engineStreamLogger << msg;
        throw std::runtime_error(msg.data());
    });

    log->RegisterCallback(nador::ELogType::DEBUG, [standardLogger, userstreamLogger](std::string_view msg) mutable {
        standardLogger.Debug(msg);
        userstreamLogger << msg;
    });
    log->RegisterCallback(nador::ELogType::WARNING, [standardLogger, userstreamLogger](std::string_view msg) mutable {
        standardLogger.Warning(msg);
        userstreamLogger << msg;
    });
    log->RegisterCallback(nador::ELogType::ERROR, [standardLogger, userstreamLogger](std::string_view msg) mutable {
        standardLogger.Error(msg);
        userstreamLogger << msg;
    });
    log->RegisterCallback(nador::ELogType::FATAL, [standardLogger, userstreamLogger](std::string_view msg) mutable {
        standardLogger.Fatal(msg);
        userstreamLogger << msg;
        throw std::runtime_error(msg.data());
    });

    nador::SetLoggingInterface(std::move(log));
}

int main(void)
{
    // Logger setup
    //SetupLogging();

    std::chrono::milliseconds ms{};
    nador::utils::MeasureTime(ms, SetupLogging);

    NADOR_DEBUG("SetupLogging duration: %dms", ms.count());

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