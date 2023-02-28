#ifndef __SIMPLE_ATLAS_GENERATOR_H__
#define __SIMPLE_ATLAS_GENERATOR_H__

#include "SimpleImageManager.hpp"
#include "SimpleAtlas.hpp"

namespace am {

	namespace sp {

		enum LEVEL {
			EASY_HEIGHT,
			EASY_WIDTH,
			EASY_AREA,
			HARD
		};

		using simpl_atlas_cont_t = std::vector<SimpleAtlas<>>;

		class SimpleAtlasGenerator {
		public:
			SimpleAtlasGenerator(const SimpleImageManager<>& manager, const math::Vec2d<int>& atlas_dims);

			simpl_atlas_cont_t GenerateSimpleAtlases(const LEVEL level);

		private:
			simpl_atlas_cont_t _GenerateAtlases(const SORT_TYPE type);

			SimpleImageManager<>	_img_manager;
			math::Vec2d<int>		_atlas_dims;
		};

	}

}



#endif //!__SIMPLE_ATLAS_GENERATOR_H__
