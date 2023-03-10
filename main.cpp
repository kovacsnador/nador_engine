#include <iostream>

#include "libnador/nador/App.h"

enum Fonts : uint32_t
{
	FREE_SANS = 1,
};

enum FontSize : uint32_t
{
	SMALL = 32,
	MEDIUM = 64,
	LARGE = 128,
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

	fontCtrl->RegisterFont(Fonts::FREE_SANS, "FreeSans.ttf");

	//Sets the default font size
	fontCtrl->SetDefaultSystemFont(Fonts::FREE_SANS, FontSize::SMALL);

	while (fontCtrl->IsLoading())
	{
	}

	auto diff = std::chrono::system_clock::now() - start;
	NADOR_DEBUG(0, "InitFonts duration: %d ms", std::chrono::duration_cast<std::chrono::milliseconds>(diff));
}

void InitSounds()
{
	nador::ISoundController* soundCtrl = nador::IApp::Get()->GetSoundController();

	soundCtrl->AddSound("TestSound.wav", Sound::TEST_SOUND_1);
	soundCtrl->AddSound("TestSound_Mono.wav", Sound::TEST_SOUND_2);
}

int main(void)
{
	NADOR_LOG.RegisterCallback(nador::ELogType::DEBUG, [](const char* msg) { std::cout << "\033[1;32m" << msg << "\033[0m" << std::endl; });
	NADOR_LOG.RegisterCallback(nador::ELogType::WARNING, [](const char* msg) { std::cout << "\033[1;33m" << msg << "\033[0m" << std::endl; });
	NADOR_LOG.RegisterCallback(nador::ELogType::ERROR, [](const char* msg) { std::cout << "\033[1;31m" << msg << "\033[0m" << std::endl; });

	NADOR_LOG.EnableFileName(true);
	NADOR_LOG.EnableFuncName(false);

	//Initialize the app with the config settings.
	nador::AppConfig config = nador::ReadAppConfig("app_config.xml");

	// Create the main Application.
	nador::App app(config);

	InitFonts();
	InitSounds();

	//Game loop
	app.Run();

	return 0;
}