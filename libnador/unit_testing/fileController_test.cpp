#include <filesystem>

#include <gtest/gtest.h>

#include "nador/system/ModuleFactory.h"

namespace
{
    using PathPair_t = std::pair<std::filesystem::path, std::filesystem::path>;
}

struct FileControllerTest : public ::testing::TestWithParam<PathPair_t>
{
    void SetUp() override
    {
        fileCtrl = nador::ModuleFactory::CreateFileController("");
        const auto& [root, _] = GetParam();
        EXPECT_TRUE(std::filesystem::create_directories(root));
    }

    void TearDown() override
    {
        const auto& [root, _] = GetParam();
        EXPECT_TRUE(std::filesystem::remove_all(root));
    }

    nador::IFileControllerUPtr fileCtrl {nullptr};
};

INSTANTIATE_TEST_CASE_P(FileControllerTest, FileControllerTest, ::testing::Values(
    PathPair_t{"root" , "unit_test_file.txt"},
    PathPair_t{"root" , "unit_test_file2.txt"},
    PathPair_t{"root" , "unit_test_file3.txt"}));

TEST_P(FileControllerTest, WriteAndRead_test)
{
    const auto& [root, file] = GetParam();
    auto fullPath = root / file;

    std::string_view helloWorld = "Hello World!";

    // write the file
    EXPECT_TRUE(fileCtrl->Write(fullPath, helloWorld.begin(), helloWorld.size()));

    // read the file
    auto fileReaded = fileCtrl->Read(fullPath);
    EXPECT_TRUE(fileReaded && fileReaded->IsValid());

    std::string_view readedData{fileReaded->Begin(), fileReaded->GetSize()};

    EXPECT_EQ(readedData.size(), helloWorld.size());
    EXPECT_TRUE(readedData == helloWorld);

    //delete the file
    EXPECT_TRUE(fileCtrl->Delete(fullPath));
}

TEST_P(FileControllerTest, FileControllerTest_test)
{
    const auto& [root, file] = GetParam();

    fileCtrl->SetRootPath(root);

    std::string_view helloWorld = "Hello World!";

    EXPECT_TRUE(fileCtrl->Write(file, helloWorld.begin(), helloWorld.size()));

    // read the file
    auto fileReaded = fileCtrl->Read(file);
    EXPECT_TRUE(fileReaded && fileReaded->IsValid());

    std::string_view readedData{fileReaded->Begin(), fileReaded->GetSize()};

    EXPECT_EQ(readedData.size(), helloWorld.size());
    EXPECT_TRUE(readedData == helloWorld);

    //delete the file
    EXPECT_TRUE(fileCtrl->Delete(file));

    // check if it's deleted
    auto deletedFile = fileCtrl->Read(file);
    EXPECT_FALSE(deletedFile);
}