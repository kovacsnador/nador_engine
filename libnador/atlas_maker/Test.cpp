#include <iostream>

#include "Test.h"
#include "SimpleAtlas.hpp"
#include "FileUtil.h"
#include "RefCountTestClass.hpp"
#include "ImageManager.hpp"
#include "IImage.h"
#include "SimpleAtlasGenerator.h"
#include "AtlasImage.h"

namespace am {

	void Test::Test1() {

		//RefCountTest();


		//am::file::path_contener_t test = am::file::FileUtil::SearchFilesInFolder("C:\\Users\\abasa\\Desktop\\atlas_test", am::file::PNG_SEGMENT, true);
		//am::file::path_contener_t test2 = am::file::FileUtil::SearchSubFolders("C:\\Users\\abasa\\Desktop\\atlas_test", true);


		am::file::img_package_t package = am::file::FileUtil::SearchPackages("C:\\Users\\abasa\\Desktop\\atlas_test\\one_atlas_test", am::file::FileUtil::GetImageSegment());

		api::ImageManager manager(package.at(0));

		std::vector<id_t> ids = manager.GetAllId();

		//png::PngUtil::WritePngFile("C:\\Users\\abasa\\Desktop\\atlas_test\\test.png", *dynamic_cast<const Image*>(api::ImageContainer<>::GetInstance().GetById(1)));

		sp::SimpleImageManager<> clone = manager.GetSimpleClone();

		clone.Sort(sp::BY_HEIGHT);

		sp::SimpleAtlasGenerator atlas_generator(clone, { 512, 512 });

		sp::simpl_atlas_cont_t atlases = atlas_generator.GenerateSimpleAtlases(sp::EASY_AREA);

		//png::Atlas test_atlas("C:\\Users\\abasa\\Desktop\\atlas_test\\first_atlas.png", atlases[3]);

		//AtlasImage test_atlas_image0("C:\\Users\\abasa\\Desktop\\atlas_test\\one_atlas_test", atlases[0]);
		am::AtlasImage::CreateAtlasImage("C:\\Users\\abasa\\Desktop\\atlas_test\\one_atlas_test", atlases[0]);

		std::string file_path;
		int atlas_size = 0;

		//std::cout << "Atlas sizes :";
		//std::cin >> atlas_size;
		//std::cout << "File path :";
		//std::cin >> file_path;

		atlas_size = 10;

		am::sp::SimpleAtlas<> simple_atlas(atlas_size, atlas_size);
		am::sp::SimpleImage image(10, 11, 0);
		if (simple_atlas.PutImageInAtlas(image)) {
			std::cout << "image is in simple atlas;";
		}
		else {
			std::cout << "image is not in simple atlas";
		}

	}


	void 
	Test::RefCountTest() {

		test::RefTest* r1 = new test::RefTest();
		test::RefTest* r2 = new test::RefTest();
		test::RefTest* r3 = new test::RefTest();

		std::vector<test::RefTest> vector;

		vector.push_back(*r1);
		vector.push_back(*r2);
		vector.push_back(*r3);

		std::vector<test::RefTest> vector_copy;

		vector_copy = vector;
	}


	void 
	Test::Test2() {

		const char* const ABS_PATH = "C:\\Users\\abasa\\Desktop\\atlas_test";

		am::file::img_package_t package = am::file::FileUtil::SearchPackages("C:\\Users\\abasa\\Desktop\\atlas_test", am::file::FileUtil::GetImageSegment());

		for (const auto& pack_it : package) {
			api::ImageManager manager(pack_it);
			sp::SimpleImageManager<> clone = manager.GetSimpleClone();
			clone.Sort(sp::BY_HEIGHT);
			sp::SimpleAtlasGenerator atlas_generator(clone, { 2048, 2048 });
			sp::simpl_atlas_cont_t simple_atlases = atlas_generator.GenerateSimpleAtlases(sp::EASY_AREA);
			for (const auto& it : simple_atlases) {
				am::AtlasImage::CreateAtlasImage(ABS_PATH, it);
			}
		}

	}

	void
	Test::CimgTest() {
		const char* jpeg_path = "C:\\Users\\abasa\\Desktop\\atlas_test\\atlas2\\jpegtest\\jpgtest.jpg";
		const char* png_path = "C:\\Users\\abasa\\Desktop\\atlas_test\\atlas2\\jpegtest\\png_test.png";
		const char* new_image = "C:\\Users\\abasa\\Desktop\\atlas_test\\atlas2\\jpegtest\\jpgtest_save.jpeg";

		const char* atlas_path_xyc = "C:\\Users\\abasa\\Desktop\\atlas_test\\atlas2\\jpegtest\\atlas_save_xyc.jpeg";
		const char* atlas_path_xyz = "C:\\Users\\abasa\\Desktop\\atlas_test\\atlas2\\jpegtest\\atlas_save_xyz.jpeg";
		const char* atlas_path_xzc = "C:\\Users\\abasa\\Desktop\\atlas_test\\atlas2\\jpegtest\\atlas_save_xzc.jpeg";
		const char* atlas_path_yzc = "C:\\Users\\abasa\\Desktop\\atlas_test\\atlas2\\jpegtest\\atlas_save_yzc.jpeg";

		cimg_library::CImg<byte_t> img(jpeg_path);
		cimg_library::CImg<byte_t> img_png(png_path);
		img_png.channels(0, 2);
		//img.channels(0, 2);                   // Force 4th channel.
		//img.get_shared_channel(2).fill(255); // Fill it with 255.
		//img.save_jpeg(new_image);

		cimg_library::CImg<byte_t> atlas_xyc(400, 400, 1, 3, 0);
		cimg_forXYC(img, x, y, c) {  // Do 3 nested loops
			atlas_xyc(x, y, c) = img(x, y, c);
		}
		atlas_xyc.save_jpeg(atlas_path_xyc);

		cimg_library::CImg<byte_t> atlas_xyc_png(400, 400, 1, 3, 0);
		cimg_forXYC(img_png, x, y, c) {  // Do 3 nested loops
			atlas_xyc_png(x, y, c) = img_png(x, y, c);
		}
		atlas_xyc_png.save_jpeg(atlas_path_xyc);

		cimg_library::CImg<byte_t> atlas_xyz(200, 200, 1, 3);
		cimg_forXYZ(img, x, y, z) {
			atlas_xyz(x, y, z) = img(x, y, z);
		}
		atlas_xyz.save_jpeg(atlas_path_xyz);

		cimg_library::CImg<byte_t> atlas_xzc(200, 200, 1, 3);
		cimg_forXZC(img, x, z, c) {
			atlas_xzc(x, z, c) = img(x, z, c);
		}
		atlas_xzc.save_jpeg(atlas_path_xzc);

		cimg_library::CImg<byte_t> atlas_yzc(200, 200, 1, 3);
		cimg_forYZC(img, y, z, c) {
			atlas_yzc(y, z, c) = img(y, z, c);
		}
		atlas_yzc.save_jpeg(atlas_path_yzc);
		
		exit(0);
	}

}
