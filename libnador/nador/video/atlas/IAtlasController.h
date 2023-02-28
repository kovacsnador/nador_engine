#ifndef __I_ATLAS_CONTROLLER_H__
#define __I_ATLAS_CONTROLLER_H__

#include <vector>

#include "nador/video/atlas/Atlas.h"

namespace nador
{
	class IAtlasController
	{
	public:
		virtual ~IAtlasController() = default;
	
	public:
		using atlases_t = std::vector<AtlasPtr>;

		/*!
		 * Gets the image data.
		 *
		 * \param name	The image id.
		 *
		 * \return	The Image data.
		 */
		virtual ImageData GetImage(video::EImageName name) const = 0;

		/*!
		 * Gets all atlases.
		 *
		 * \return	The atlases in container.
		 */
		virtual const atlases_t& GetAtlases() const = 0;

		/*!
		 * Gets all atlas names.
		 *
		 * \return	The atlases names.
		 */
		virtual strings_t GetAtlasNames() const = 0;
	};
	CREATE_PTR_TYPES(IAtlasController);
}

#endif // !__I_ATLAS_CONTROLLER_H__
