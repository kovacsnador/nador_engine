#include "SimpleAtlasGenerator.h"
#include "FileUtil.h"


namespace am {

	namespace sp {

		SimpleAtlasGenerator::SimpleAtlasGenerator(const SimpleImageManager<>& manager, const math::Vec2d<int>& atlas_dims)
			: _img_manager(manager)
			, _atlas_dims(atlas_dims)
		{
			//no func
		}

		simpl_atlas_cont_t 
		SimpleAtlasGenerator::GenerateSimpleAtlases(const LEVEL level) {
			return _GenerateAtlases(BY_AREA);
		}

		simpl_atlas_cont_t
		SimpleAtlasGenerator::_GenerateAtlases(const SORT_TYPE type) {
			_img_manager.Sort(type);
			simpl_atlas_cont_t container;
			SimpleAtlas<> atlas_temp(_atlas_dims.X(), _atlas_dims.Y());
			for (const auto& it : _img_manager.GetContainer()) {
				if (!atlas_temp.PutImageInAtlas(it)) {
					container.push_back(atlas_temp);
					atlas_temp.Clear();
					if (!atlas_temp.PutImageInAtlas(it)) {
						throw std::runtime_error(file::FileUtil::SF("Cant write image %d in atlas", it.GetId()));
					}
				}
			}
			if (!atlas_temp.Empty()) {
				container.push_back(atlas_temp);
			}
			return std::move(container);
		}


	}


}