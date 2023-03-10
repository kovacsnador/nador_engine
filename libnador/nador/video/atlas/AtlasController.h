#ifndef __ATLAS_CONTROLLER_H__
#define __ATLAS_CONTROLLER_H__

#include <vector>
#include <memory>

#include "nador/video/atlas/IAtlasController.h"

namespace nador
{
	class AtlasController : public IAtlasController
	{
		using image_lookup_t = std::map<video::EImageName, size_t>;
	
	public:
		/*!
		 * AtlasController construcor.
		 */
		AtlasController(const DataPtr& fileData);

		/*!
		 * Gets the image data.
		 *
		 * \param name	The image id.
		 *
		 * \return	The Image data.
		 */
		ImageData GetImage(video::EImageName name) const override;

		/*!
		 * Gets all atlases.
		 *
		 * \return	The atlases in container.
		 */
		const atlases_t& GetAtlases() const override;

		/*!
		 * Gets all atlas names.
		 *
		 * \return	The atlases names.
		 */
		strings_t GetAtlasNames() const override;

	private:
		image_lookup_t	_imageLookup;
		atlases_t		_atlases;
	};

	CREATE_PTR_TYPES(AtlasController);
}

#endif // !__ATLAS_CONTROLLER_H__

