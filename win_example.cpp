#include <iostream>

#include "nador/App.h"
#include "nador/log/StandardLogger.h"
#include "nador/log/StreamLogger.h"
#include "nador/common/Stopwatch.h"

#include "Game.h"

enum Fonts : uint32_t
{
    FREE_SANS = 1,
    SUPER_MARIO_BROS_3
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

    MARIO_THEME,
    SMB_1_UP,
    SMB_BOWSERFALLS,
    SMB_BOWSERFIRE,
    SMB_BREAKBLOCK,
    SMB_BUMP,
    SMB_COIN,
    SMB_FIREBALL,
    SMB_FIREWORKS,
    SMB_FLAGPOLE,
    SMB_GAMEOVER,
    SMB_JUMP_SMALL,
    SMB_JUMP_SUPER,
    SMB_KICK,
    SMB_MARIODIE,
    SMB_PAUSE,
    SMB_PIPE,
    SMB_POWERUP_APPEARS,
    SMB_POWERUP,
    SMB_STAGE_CLEAR,
    SMB_STOMP,
    SMB_VINE,
    SMB_WARNING,
    SMB_WORLD_CLEAR
};

static void InitFonts()
{
    nador::Stopwatch sw;

    nador::IFontController* fontCtrl = nador::IApp::Get()->GetFontController();
    nador::IFileController* fileCtrl = nador::IApp::Get()->GetFileController();

    fontCtrl->AddFontSize(FontSize::SMALL);
    fontCtrl->AddFontSize(FontSize::MEDIUM);
    fontCtrl->AddFontSize(FontSize::LARGE);

    fontCtrl->CreateFont(Fonts::FREE_SANS, fileCtrl->Read("res/fonts/FreeSans.ttf"));
    fontCtrl->CreateFont(Fonts::SUPER_MARIO_BROS_3, fileCtrl->Read("res/fonts/Super-Mario-Bros--3.ttf"));

    // Sets the default font size
    fontCtrl->SetDefaultSystemFont(Fonts::FREE_SANS, FontSize::SMALL);

    fontCtrl->Wait();

    NADOR_DEBUG("InitFonts duration: %d ms", sw.Stop<std::chrono::milliseconds>().count());
}

static void InitSounds()
{
    nador::Stopwatch sw;

    nador::ISoundController* soundCtrl = nador::IApp::Get()->GetSoundController();
    nador::IFileController*  fileCtrl  = nador::IApp::Get()->GetFileController();

    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/TestSound.wav"), Sound::TEST_SOUND_1);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/TestSound_Mono.wav"), Sound::TEST_SOUND_2);

    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/mario_theme.mp3"), MARIO_THEME);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_1-up.wav"), SMB_1_UP);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_bowserfalls.wav"), SMB_BOWSERFALLS);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_bowserfire.wav"), SMB_BOWSERFIRE);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_breakblock.wav"), SMB_BREAKBLOCK);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_bump.wav"), SMB_BUMP);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_coin.wav"), SMB_COIN);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_fireball.wav"), SMB_FIREBALL);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_fireworks.wav"), SMB_FIREWORKS);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_flagpole.wav"), SMB_FLAGPOLE);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_gameover.wav"), SMB_GAMEOVER);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_jump-small.wav"), SMB_JUMP_SMALL);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_jump-super.wav"), SMB_JUMP_SUPER);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_kick.wav"), SMB_KICK);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_mariodie.wav"), SMB_MARIODIE);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_pause.wav"), SMB_PAUSE);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_pipe.wav"), SMB_PIPE);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_powerup_appears.wav"), SMB_POWERUP_APPEARS);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_powerup.wav"), SMB_POWERUP);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_stage_clear.wav"), SMB_STAGE_CLEAR);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_stomp.wav"), SMB_STOMP);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_vine.wav"), SMB_VINE);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_warning.wav"), SMB_WARNING);
    soundCtrl->LoadSound(fileCtrl->Read("res/sounds/mario/smb_world_clear.wav"), SMB_WORLD_CLEAR);

    NADOR_DEBUG("InitSounds duration: %d ms", sw.Stop<std::chrono::milliseconds>().count());
}

void SetupLogging()
{
    auto log = std::make_unique<nador::Log<>>();

    nador::StandardLogger standardLogger;

    auto                engineStream = nador::GetStream<std::ofstream>("logs/nador_engine.log");
    auto                engineBuffer = std::make_shared<nador::StreamBuffer<20000>>();
    nador::StreamLogger engineStreamLogger(engineStream, engineBuffer);

    auto                userStream = nador::GetStream<std::ofstream>("logs/nador.log");
    auto                userBuffer = std::make_shared<nador::StreamBuffer<20000>>();
    nador::StreamLogger userstreamLogger(userStream, userBuffer);

    auto engineLogCallback = [standardLogger, engineStreamLogger](nador::ELogType logType, auto event) mutable {
        if (std::holds_alternative<std::string_view>(event))
        {
            auto msg = std::get<std::string_view>(event);
            standardLogger.Write(logType, msg);
            engineStreamLogger.Write(msg);

            if (logType == nador::ELogType::ENGINE_FATAL)
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

            if (logType == nador::ELogType::FATAL)
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
    std::chrono::milliseconds ms {};
    nador::utils::MeasureTime(ms, SetupLogging);

    NADOR_DEBUG("SetupLogging duration: %dms", ms.count());

    {
        // Initialize the app with the config settings.
        nador::AppConfig config = nador::ReadAppConfigFromFile("app_config.xml");

        // Create the main Application.
        nador::IAppPtr app = nador::App::CreateApp(config);

        // create demo game
        auto demoGame = demo::CreateGame(app);

        // Adding default tests
        app->InitializeDefaultTests();

        // add game tougle
        auto testCtrl = app->GetTestController();
        testCtrl->AddCustomButton("Toggle Game", [&demoGame]() { demoGame->Suspend(!demoGame->IsSuspended()); });

        InitFonts();
        InitSounds();

        // Application loop
        app->Run();
    }

    return 0;
}