#ifndef __ATLAS_CONTROLLER_H__
#define __ATLAS_CONTROLLER_H__

#include <vector>
#include <memory>
#include <unordered_map>

#include "nador/video/atlas/IAtlasController.h"
#include "nador/config/AppConfig.h"
#include "nador/utils/LRUCache.hpp"
#include "nador/video/atlas/AtlasConfigParser.h"

namespace nador
{
    class AtlasController : public IAtlasController
    {
        using image_lookup_t = std::unordered_map<video::EImageName, size_t>;

    public:
        /*!
         * AtlasController construcor.
         */
        AtlasController(IAtlasController::AtlasList_t atlasList, size_t cacheSize);

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
        const AtlasList_t& GetAtlases() const override;

        /*!
         * Gets all atlas names.
         *
         * \return	The atlases names.
         */
        strings_t GetAtlasNames() const override;

    private:
        image_lookup_t    _imageLookup;
        const AtlasList_t _atlases;

        mutable LRUCache<size_t, Atlas*> _cache;
    };

    CREATE_PTR_TYPES(AtlasController);
} // namespace nador

#endif // !__ATLAS_CONTROLLER_H__
