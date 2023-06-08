#include <gtest/gtest.h>

#include "nador/common/AppConfig.h"

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

                        <VideoSettings>
		                    <MaxVertexCount> 10000 </MaxVertexCount>
	                    </VideoSettings>
	
	                    <RootFilePath></RootFilePath>
	                    <UiPath> res/ui/ </UiPath>
                    </AppConfig>)",
                    {
                        {   /*WindowSettings*/
                            "Nador Application",    /*AppName*/
                            { 1024, 768 },          /*WindowDimensions*/
                            false,                  /*vSync*/
                            true,                   /*ShowDebugWindow*/
                            true                    /*ShowDebugInfo*/
                        },
                        {   /*AtlasSettings*/
                            "res/atlas/__atlases/atlas_config.json",    /*AtlasConfigPath*/
                            "res/atlas/__atlases/"                      /*AtlasImagesPath*/
                        },
                        {   /*VideoSettings*/
                            10000,      /*MaxVertexCount*/
                        },
                        "",         /*RootFilePath*/
                        "res/ui/",  /*UiPath*/
                    }
                }
    ));

TEST_P(NadorConfigTest, NadorConfigTest_1)
{
    nador::AppConfig appConfig = nador::ReadAppConfig(GetParam().rawConfigData);
    EXPECT_EQ(appConfig, GetParam().configExpected);
}