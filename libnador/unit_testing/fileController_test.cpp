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
        const auto& [path, _] = GetParam();

        std::cout << "Create path: " << path << '\n';
        EXPECT_TRUE(std::filesystem::create_directories(path));
    }

    void TearDown() override
    {
        const auto& [path, _] = GetParam();

        std::cout << "Delete path: " << path << '\n';
        std::cout << "Delete parent path: " << path.parent_path() << '\n';
        std::cout << "Delete root path: " << path.root_directory() << '\n';
        std::cout << "Root name path: " << path.root_name() << '\n';

        // getting parent folder if exist
        auto p = path;
        while(p.has_parent_path())
        {
            p = p.parent_path();
        }

        // cleanup test folder
        EXPECT_TRUE(std::filesystem::remove_all(p));
    }

    nador::IFileControllerUPtr fileCtrl {nullptr};
};

INSTANTIATE_TEST_CASE_P(FileControllerTest, FileControllerTest, ::testing::Values(
    PathPair_t{"unit_test" , "unit_test_file.txt"},
    //PathPair_t{"unit_test/root" , "unit_test_file2.txt"},
    PathPair_t{"unit_test/root/sub" , "unit_test_file3.txt"}));

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

TEST_P(FileControllerTest, IsExistAndDelete_test)
{
    auto [root, file] = GetParam();

    auto subfolder = root / "newFolder";

    EXPECT_TRUE(fileCtrl->MakeDirs(subfolder));
    EXPECT_TRUE(fileCtrl->IsExist(subfolder));

    std::string_view helloWorld = "Hello World!";
    EXPECT_TRUE(fileCtrl->Write(subfolder / file, helloWorld.begin(), helloWorld.size()));
    EXPECT_TRUE(fileCtrl->IsExist(subfolder / file));
    EXPECT_TRUE(fileCtrl->Delete(subfolder / file));
    EXPECT_FALSE(fileCtrl->IsExist(subfolder / file));

    EXPECT_TRUE(fileCtrl->Write(subfolder / file, helloWorld.begin(), helloWorld.size()));
    EXPECT_TRUE(fileCtrl->IsExist(subfolder / file));

    EXPECT_TRUE(fileCtrl->Delete(subfolder));
    EXPECT_FALSE(fileCtrl->IsExist(subfolder / file));
    EXPECT_FALSE(fileCtrl->IsExist(subfolder));
}   