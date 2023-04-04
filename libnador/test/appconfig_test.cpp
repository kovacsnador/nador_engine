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
	                <AppName> Nador Application </AppName>
	                <WindowDimensions> 1024 768 </WindowDimensions>
	                <VSync> false </VSync>

	                <RootFilePath></RootFilePath>
	                <AtlasConfigPath> res/atlas/__atlases/atlas_config.json </AtlasConfigPath>
	                <AtlasImagesPath> res/atlas/__atlases/ </AtlasImagesPath>
	                <TexturesPath></TexturesPath>
	                <UiPath> res/ui/ </UiPath>
	
	                <ShowDebugWindow> true </ShowDebugWindow>
	                <ShowDebugInfo> true </ShowDebugInfo>
                    </AppConfig>)", 
                    
                    {
                        "Nador Application",
                        { 1024, 768 },
                        false,
                        "",
                        "res/atlas/__atlases/atlas_config.json",
                        "res/atlas/__atlases/",
                        "res/ui/",
                        true,
                        true
                    }
                }
    ));

TEST_P(NadorConfigTest, NadorConfigTest_1)
{
    nador::AppConfig appConfig = nador::ReadAppConfig(GetParam().rawConfigData);
    EXPECT_EQ(appConfig, GetParam().configExpected);
}