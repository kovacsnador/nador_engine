#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <document.h>
#include <writer.h>
#include <stringbuffer.h>
#include <regex>

#include "FileUtil.h"
#include "SimpleAtlas.hpp"
#include "ImageContainer.hpp"
#include "Test.h"
#include "RefCountTestClass.hpp"
#include "ImageManager.hpp"
#include "IImage.h"
#include "SimpleAtlasGenerator.h"
#include "AtlasImage.h"

#include "CImg.h"

using thread_container_t = std::vector<std::thread>;
const am::IMAGE_TYPE ATLAS_TYPE = am::IMAGE_TYPE::PNG;				//supports only PNG for atlases
typedef bool(*StringCallback)(char c);

void
GenerateAtlasesFromPackage(const am::file::path_contener_t& pack_it, const std::string& path, const int size) {
	am::api::ImageManager manager(pack_it);
	am::sp::SimpleImageManager<> clone = manager.GetSimpleClone();
	clone.Sort(am::sp::BY_AREA);
	am::sp::SimpleAtlasGenerator atlas_generator(clone, { size, size });
	am::sp::simpl_atlas_cont_t simple_atlases = atlas_generator.GenerateSimpleAtlases(am::sp::EASY_AREA);
	for (const auto& it : simple_atlases) {
		am::AtlasImage::CreateAtlasImage(path, it, ATLAS_TYPE);
	}
}

int
DeleteAtlasConfigFilesAndFolder(const std::string& path) {
	std::cout << "old generated atlases deleting..." << std::endl;
	return am::file::FileUtil::DeleteFolder(path);
}

void
WaitForThreads(thread_container_t& threads) {
	for (auto& it : threads) {
		if (it.joinable()) {
			it.join();
		}
	}
}

void 
SaveConfigInFile(const am::file::path_contener_t& config_name, const std::string& folder) {
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value array(rapidjson::kArrayType);
	for (size_t i = 1; i < config_name.size();  ++i) {
		rapidjson::Value config_obj(rapidjson::kObjectType);
		std::string c_name = config_name[i];
		std::string i_name = am::file::FileUtil::GetFileName(c_name) + am::GetPrefix(ATLAS_TYPE);
		config_obj.AddMember(rapidjson::Value("c_name", allocator), rapidjson::Value(c_name.c_str(), allocator), allocator);
		config_obj.AddMember(rapidjson::Value("i_name", allocator), rapidjson::Value(i_name.c_str(), allocator), allocator);
		array.PushBack(config_obj, allocator);
	}
	document.AddMember("atlas_names", array, allocator);
	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);
	am::file::FileUtil::SaveFile((folder + "\\atlas_config").c_str(), (am::byte_t*)strbuf.GetString(), strbuf.GetLength(), am::JSON_PREFIX);
}

bool
ValidNameChar(char c) {
	if (c > 47 && c < 58) {
		return true;	//is nummer
	}
	if (c > 64 && c < 91) {
		return true;	//is big letter
	}
	if (c > 96 && c < 123) {
		return true;	//is small letter
	}
	return false;
}

std::string
RemoveNotValidChar(const std::string& str, StringCallback callback) {
	std::string temp;
	for (size_t i = 0; i < str.size(); ++i) {
		if (callback(str[i])) {
			temp += toupper(str[i]);
		}
	}
	return temp;
}

void
GenerateTheImageEnumFile(const std::string& folder) {
	const char* const file_start = "#ifndef __E_IMAGE_NAME_H__\n#define __E_IMAGE_NAME_H__\n\nnamespace nador {\n\n\tnamespace video {\n\n\t\tenum class EImageName {";
	const char* const file_end = "\n\n\t}\n\n}\n\n#endif //__E_IMAGE_NAME_H__!";
	std::string file = file_start;
	size_t imageCount = 0;
	for (size_t i = 0; i < IMAGE_CONTAINER.GetSize(); ++i) {
		const am::IImage* img = IMAGE_CONTAINER.GetById(i);
		if (!img->IsAtlas()) {
			auto imgId = img->GetId();
			std::string str = RemoveNotValidChar(img->GetName(), ValidNameChar);
			file += "\n\t\t\t" + str;
			file += " = " + std::to_string(imgId) + ",";
			imageCount = imgId + 1;
		}
	}
	file += "\n\t\t\tIMAGE_NAME_COUNT = " + std::to_string(imageCount) + "\n\t\t};";
	file += file_end;

	am::file::FileUtil::SaveFile((folder + "\\EImageName").c_str(), (am::byte_t*)file.c_str(), file.size(), ".h");
}

int
GenerateAtlasesConfigFile(const std::string& path) {
	std::cout << "Generating atlas config file..." << std::endl;
	am::file::segment_container_t segments;
	segments.push_back(am::file::JSON_SEGMENT);
	am::file::path_contener_t files = am::file::FileUtil::SearchFilesInFolder(path, segments, false);
	SaveConfigInFile(files, path);
	GenerateTheImageEnumFile(path);
	std::cout << "Atlas config file is saved" << std::endl;
	return files.size();
}

void 
GenerateAtlases(const std::string& path, const int size) {
	std::cout << "Generate atlases..." << std::endl;
	std::string atlas_config_path = path + "\\" + am::ATLAS_FOLDER_NAME;
	DeleteAtlasConfigFilesAndFolder(atlas_config_path);
	std::cout << "Old generated atlases deleted!" << std::endl;
	std::chrono::system_clock::time_point start(std::chrono::system_clock::now());
	am::file::segment_container_t segments;
	am::file::img_package_t package = am::file::FileUtil::SearchPackages(path, am::file::FileUtil::GetImageSegment());
	thread_container_t threads;
	for (const auto& pack_it : package) {
		std::cout << "Starting task..." << std::endl;
		threads.push_back(std::thread(GenerateAtlasesFromPackage, pack_it, path, size));
	}
	WaitForThreads(threads);
	int atlas_count = GenerateAtlasesConfigFile(atlas_config_path) - 1;
	std::cout << std::endl << std::endl << "------------------- TOTAL --------------------------" << std::endl;
	std::cout << "Total time: " << static_cast<double>((std::chrono::system_clock::now() - start).count()) / 10000000 << "sec\n";
	if (IMAGE_CONTAINER.GetSize() && atlas_count > 0) {
		std::cout << "Total images: " << IMAGE_CONTAINER.GetSize() - atlas_count << std::endl;
		std::cout << "Total atlases: " << atlas_count << std::endl;
	}
}

int main()
{
	//------------------ TEST -------------
	//am::Test::Test1();
	//am::Test::Test2();

	//am::Test::CimgTest();

	//---------------------------------

	//std::regex path_regex(".*\\\\(.*)\\..*$");
	std::regex path_regex("C:*");

	std::string path;
	int size;
	bool next = true;
	do {
		std::cout << "Path from atlases: ";
		std::cin >> path;
		if (!std::cin) {
			next = false;
			std::cout << "Path not valid!" << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else {
			next = true;
		}
	} while (!next);

	do {
		std::cout << "Atlas size(for example : 512, 1024, 2048): ";
		std::cin >> size;
		if (!std::cin) {
			next = false;
			std::cout << "Atlas size not valid!" << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else {
			next = true;
		}
	} while (!next);

	//path = "C:\\Users\\abasa\\Desktop\\atlas_test";
	//size = 2048;

	GenerateAtlases(path, size);

	system("pause");
	return 0;
}