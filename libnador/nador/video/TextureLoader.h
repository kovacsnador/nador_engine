#ifndef __TEXTURE_LOADER_H__
#define __TEXTURE_LOADER_H__

#include <memory>

#include "nador/utils/Types.h"
#include "nador/system/IFileController.h"

namespace nador
{
	struct TextureData
	{
		/*!
		 * TextureData constructor.
		 *
		 * \param filePath	The texture file path.
		 */
		//TextureData(const char* filePath);

		/*!
		 * TextureData constructor.
		 *
		 * \param data	The texture data.
		 */
		TextureData(const DataPtr& data);

		/*!
		 * TextureData destructor.
		 */
		~TextureData();

		uint8_t* localBuffer;
		int32_t  width;
		int32_t  height;
		int32_t  pbb;

	private:
		/*!
		 * Creates the texture data.
		 *
		 * \param data	The texture data.
		 */
		void _CreateTextureData(const DataPtr& data);
	};

	CREATE_PTR_TYPES(TextureData);


	class TextureLoader
	{
	public:
		/*!
		 * Load a texture from file.
		 *
		 * \param filePath	The texture file path.
		 *
		 * \return		The raw texture data.
		 */
		//static TextureDataPtr LoadFromFile(const char* filePath);

		/*!
		 * Load a texture from buffer.
		 *
		 * \param data	The texture buffer.
		 *
		 * \return		The raw texture data.
		 */
		static TextureDataPtr LoadFromBuffer(const DataPtr& data);
	};
}

#endif // !__TEXTURE_LOADER_H__

