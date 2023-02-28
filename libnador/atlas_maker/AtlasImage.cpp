#include <document.h>
#include <writer.h>
#include <stringbuffer.h>
#include <iostream>

#include "AtlasImage.h"
#include "FileUtil.h"
#include "ImageUtils.h"

namespace am {

	static const std::string ATLAS = "atlas";

	AtlasImage* AtlasImage::CreateAtlasImage(const std::string& file_path, const sp::SimpleAtlas<>& simple_atlas, const IMAGE_TYPE type)
	{
		return new AtlasImage(file_path, simple_atlas, type);
	}

	AtlasImage::AtlasImage(const std::string& file_path, const sp::SimpleAtlas<>& simple_atlas, const IMAGE_TYPE type)
		: Image(simple_atlas.GetWidth(), simple_atlas.GetHeight(), 1, DetectChannel(type))
		, _type(type)
	{
		std::cout << "Atlas creating... name:" << _AtlasName() << std::endl;
		_is_atlas = true;
		_file_path = file_path;
		_GenerateAtlasFromSimpleAtlas(simple_atlas);
		_Save();
		_SaveJsonData(simple_atlas);
	}

	void
	AtlasImage::_Save() {
		std::cout << "Saving atlas: " << _AtlasName() << std::endl;
		file::FileUtil::CreateFolder(_file_path + "\\" + ATLAS_FOLDER_NAME);
		std::string full_name(_PutNameTogether(_file_path, ATLAS_FOLDER_NAME));
		switch (_type) {
		case IMAGE_TYPE::PNG:
			_image.save_png((full_name += PNG_PREFIX).c_str());
			break;
		case IMAGE_TYPE::JPEG:
			_image.save_jpeg((full_name += JPEG_PREFIX).c_str());
			break;
		case IMAGE_TYPE::BMP:
			_image.save_bmp((full_name += BMP_PREFIX).c_str());
			break;
		default:
			throw std::runtime_error("Can't save atlas because of type!");
			break;
		}
	}

	void
	AtlasImage::_GenerateAtlasFromSimpleAtlas(const sp::SimpleAtlas<>& simple_atlas) {
		const sp::image_container_t& images = simple_atlas.GetImages();
		for (const auto& it : images) {
			this->_PutImageInAtlas(it.GetId(), it.GetPosition());
		}
	}

	void
	AtlasImage::_PutImageInAtlas(const id_t& id, const math::Vec2d<int>& pos) {
		IImage* img = IMAGE_CONTAINER.GetById(id);
		Image* image = dynamic_cast<Image*>(img);
		const img_t& cimg = image->GetImage();
		if (image->GetSprectrum() == 4) {
			cimg_forXYC(cimg, x, y, c) {  // Do 3 nested loops
				_image(x + pos.X(), y + pos.Y(), c) = cimg(x, y, c);
			}
		}
		else if (image->GetSprectrum() == 3) {
			cimg_forXY(cimg, x, y) {  // Do 2 nested loops
				_image(x + pos.X(), y + pos.Y(), 0) = cimg(x, y, 0);
				_image(x + pos.X(), y + pos.Y(), 1) = cimg(x, y, 1);
				_image(x + pos.X(), y + pos.Y(), 2) = cimg(x, y, 2);
				_image(x + pos.X(), y + pos.Y(), 3) = 255;
			}
		}
		else if (image->GetSprectrum() == 2) {
			cimg_forXY(cimg, x, y) {  // Do 2 nested loops
				_image(x + pos.X(), y + pos.Y(), 0) = cimg(x, y, 0);
				_image(x + pos.X(), y + pos.Y(), 1) = cimg(x, y, 1);
				_image(x + pos.X(), y + pos.Y(), 2) = 0;
				_image(x + pos.X(), y + pos.Y(), 3) = 255;
			}
		}
		else if (image->GetSprectrum() == 1) {
			cimg_forXY(cimg, x, y) {  // Do 2 nested loops
				_image(x + pos.X(), y + pos.Y(), 0) = cimg(x, y, 0);
				_image(x + pos.X(), y + pos.Y(), 1) = 0;
				_image(x + pos.X(), y + pos.Y(), 2) = 0;
				_image(x + pos.X(), y + pos.Y(), 3) = 255;
			}
		}
	}

	std::string
	AtlasImage::_PutNameTogether(const std::string& path, const std::string& folder) const {
		std::string result(path);
		result += "\\";
		result += folder;
		result += "\\";
		result += _AtlasName();
		return std::move(result);
	}

	std::string 
	AtlasImage::_AtlasName() const {
		return std::move(ATLAS + "_" + std::to_string(_id));
	}

	void
	AtlasImage::_SaveJsonData(const sp::SimpleAtlas<>& simple_atlas) const {
		rapidjson::Document document;
		document.SetObject();
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		rapidjson::Value array(rapidjson::kArrayType);
		const sp::image_container_t& images = simple_atlas.GetImages();
		for (const auto& it : images) {
			const IImage* img = IMAGE_CONTAINER.GetById(it.GetId());
			rapidjson::Value image_obj(rapidjson::kObjectType);
			image_obj.AddMember("Name", rapidjson::Value(img->GetName().c_str(), allocator), allocator);
			image_obj.AddMember("id", it.GetId(), allocator);
			image_obj.AddMember("atlas_name", rapidjson::Value((_AtlasName() + GetPrefix(_type)).c_str(), allocator), allocator);
			image_obj.AddMember("width", it.GetWidth(), allocator);
			image_obj.AddMember("height", it.GetHeight(), allocator);
			utils::UVCordinates uvs = utils::CalculateUVS(it, simple_atlas.GetWidth(), simple_atlas.GetHeight());
			image_obj.AddMember("UV_X_1", uvs._uv_s[0].X(), allocator);	//left top
			image_obj.AddMember("UV_Y_1", uvs._uv_s[0].Y(), allocator);
			image_obj.AddMember("UV_X_2", uvs._uv_s[1].X(), allocator);	//right top
			image_obj.AddMember("UV_Y_2", uvs._uv_s[1].Y(), allocator);
			image_obj.AddMember("UV_X_3", uvs._uv_s[2].X(), allocator);	//right down
			image_obj.AddMember("UV_Y_3", uvs._uv_s[2].Y(), allocator);
			image_obj.AddMember("UV_X_4", uvs._uv_s[3].X(), allocator);	//left down
			image_obj.AddMember("UV_Y_4", uvs._uv_s[3].Y(), allocator);
			array.PushBack(image_obj, allocator);
		}
		document.AddMember("images", array, allocator);

		rapidjson::StringBuffer strbuf;
		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		document.Accept(writer);

		file::FileUtil::SaveFile(_PutNameTogether(_file_path, ATLAS_FOLDER_NAME).c_str(), (byte_t*)strbuf.GetString(), strbuf.GetLength(), JSON_PREFIX);
	}

	AtlasImage::~AtlasImage() {
		//no func
	}

	int 
	DetectChannel(const IMAGE_TYPE type) {
		switch (type) {
		case IMAGE_TYPE::PNG:
		case IMAGE_TYPE::BMP:
			return 4;
		case IMAGE_TYPE::JPEG:
			return 3;
		}
		throw std::runtime_error("Unknow channel!");
	}

	const char* const
	GetPrefix(const IMAGE_TYPE type) {
		switch (type) {
		case IMAGE_TYPE::PNG:
			return PNG_PREFIX;
		case IMAGE_TYPE::BMP:
			return BMP_PREFIX;
		case IMAGE_TYPE::JPEG:
			return JPEG_PREFIX;
		default:
			return "";
		}
	}

}