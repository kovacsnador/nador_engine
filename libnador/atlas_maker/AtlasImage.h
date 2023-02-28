#ifndef __ATLAS_IMAGE_H__
#define __ATLAS_IMAGE_H__

#include "Types.h"
#include "Image.h"
#include "SimpleAtlas.hpp"

namespace am {

	static const char* JSON_PREFIX = ".json";

	class AtlasImage : public Image {
	public:
		static AtlasImage* CreateAtlasImage(const std::string& file_path, const sp::SimpleAtlas<>& simple_atlas, const IMAGE_TYPE type = IMAGE_TYPE::PNG);

	private:
		AtlasImage(const std::string& file_path, const sp::SimpleAtlas<>& simple_atlas, const IMAGE_TYPE type = IMAGE_TYPE::PNG);
		virtual ~AtlasImage();

		void		_Save();
		void		_GenerateAtlasFromSimpleAtlas(const sp::SimpleAtlas<>& simple_atlas);
		void		_PutImageInAtlas(const id_t& id, const math::Vec2d<int>& pos);
		std::string _PutNameTogether(const std::string& path, const std::string& folder) const;
		std::string _AtlasName() const;
		void		_SaveJsonData(const sp::SimpleAtlas<>& simple_atlas) const;

		IMAGE_TYPE _type;
	};

	int					DetectChannel(const IMAGE_TYPE type);
	const char* const	GetPrefix(const IMAGE_TYPE type);
}

#endif //!__ATLAS_IMAGE_H__
