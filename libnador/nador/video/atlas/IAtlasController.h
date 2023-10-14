#ifndef __I_ATLAS_CONTROLLER_H__
#define __I_ATLAS_CONTROLLER_H__

#include <vector>

#include "nador/video/atlas/Atlas.h"

namespace nador
{
	class IAtlasController
	{
	public:
		using AtlasList_t = std::vector<std::shared_ptr<Atlas>>;

		/*!
		 * IAtlasController destructor.
		 */
		virtual ~IAtlasController() = default;

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
		virtual const AtlasList_t& GetAtlases() const = 0;

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
