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
    nador::IFileController* fileCtrl = nador::IApp::Get()->GetFileController();

    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/TestSound.wav"), Sound::TEST_SOUND_1);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/TestSound_Mono.wav"), Sound::TEST_SOUND_2);

    NADOR_DEBUG("InitSounds duration: %d ms", sw.Stop<std::chrono::milliseconds>().count());
}

void SetupLogging()
{
    auto log = std::make_unique<nador::Log<>>();

    nador::StandardLogger standardLogger;

    auto engineStream = nador::GetStream<std::ofstream>("logs/nador_engine.log");
    auto engineBuffer = std::make_shared<nador::StreamBuffer<20000>>();
    nador::StreamLogger engineStreamLogger(engineStream, engineBuffer);


    auto userStream = nador::GetStream<std::ofstream>("logs/nador.log");
    auto userBuffer = std::make_shared<nador::StreamBuffer<20000>>();
    nador::StreamLogger userstreamLogger(userStream, userBuffer);


    auto engineLogCallback = [standardLogger, engineStreamLogger](nador::ELogType logType, auto event) mutable {
        if (std::holds_alternative<std::string_view>(event))
        {
            auto msg = std::get<std::string_view>(event);
            standardLogger.Write(logType, msg);
            engineStreamLogger.Write(msg);

            if(logType == nador::ELogType::ENGINE_FATAL)
            {
                throw std::runtime_error(msg.data());
            }
        }
        else if (std::holds_alternative<nador::ILog::FlushRequest>(event))
        {
            engineStreamLogger.FlushBuffer();
        }
    };

    auto userLogCallback = [standardLogger, userstreamLogger](nador::ELogType logType, auto event) mutable {
        if (std::holds_alternative<std::string_view>(event))
        {
            auto msg = std::get<std::string_view>(event);
            standardLogger.Write(logType, msg);
            userstreamLogger.Write(msg);

            if(logType == nador::ELogType::FATAL)
            {
                throw std::runtime_error(msg.data());
            }
        }
        else if (std::holds_alternative<nador::ILog::FlushRequest>(event))
        {
            userstreamLogger.FlushBuffer();
        }
    };


    // setup engine logging
    log->RegisterCallback(nador::ELogType::ENGINE_DEBUG, engineLogCallback);
    log->RegisterCallback(nador::ELogType::ENGINE_WARNING, engineLogCallback);
    log->RegisterCallback(nador::ELogType::ENGINE_ERROR, engineLogCallback);
    log->RegisterCallback(nador::ELogType::ENGINE_FATAL, engineLogCallback);

    // setup user logging
    log->RegisterCallback(nador::ELogType::DEBUG, userLogCallback);
    log->RegisterCallback(nador::ELogType::WARNING, userLogCallback);
    log->RegisterCallback(nador::ELogType::ERROR, userLogCallback);
    log->RegisterCallback(nador::ELogType::FATAL, userLogCallback);

    nador::SetLoggingInterface(std::move(log));

    nador::SetCrashHandler([engineStreamLogger, userstreamLogger](uint32_t error) mutable {
        engineStreamLogger.FlushBuffer();
        userstreamLogger.FlushBuffer();
        std::cerr << "Crash signal with code: " << error << '\n';
    });
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