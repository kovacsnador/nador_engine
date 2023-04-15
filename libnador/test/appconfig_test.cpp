#include <gtest/gtest.h>

#include "nador/App.h"

struct NadorConfigTestValue
{
    std::string rawConfigData;
    nador::AppConfig configExpected;
};

class NadorConfigTest : public ::testing::TestWithParam<NadorConfigTestValue>
{
};

INSTANTIATE_TEST_CASE_P(
    NadorConfigTest,
    NadorConfigTest,
    ::testing::Values(
        NadorConfigTestValue {
                    R"(<?xml version="1.0" encoding="utf-8"?>
                    <AppConfig>
	                	<WindowSettings>
		                    <AppName> Nador Application </AppName>
		                    <WindowDimensions> 1024 768 </WindowDimensions>
		                    <VSync> false </VSync>

		                    <ShowDebugWindow> true </ShowDebugWindow>
		                    <ShowDebugInfo> true </ShowDebugInfo>
	                    </WindowSettings>

	                    <AtlasSettings>
		                    <AtlasConfigPath> res/atlas/__atlases/atlas_config.json </AtlasConfigPath>
		                    <AtlasImagesPath> res/atlas/__atlases/ </AtlasImagesPath>
	                    </AtlasSettings>
	
	                    <RootFilePath></RootFilePath>
	                    <UiPath> res/ui/ </UiPath>
                    </AppConfig>)",
                    {
                        {
                            "Nador Application",
                            { 1024, 768 },
                            false,
                            true,
                            true
                        },
                        "",
                        "res/atlas/__atlases/atlas_config.json",
                        "res/atlas/__atlases/",
                        "res/ui/",
                    }
                }
    ));

TEST_P(NadorConfigTest, NadorConfigTest_1)
{
    nador::AppConfig appConfig = nador::ReadAppConfig(GetParam().rawConfigData);
    EXPECT_EQ(appConfig, GetParam().configExpected);
}