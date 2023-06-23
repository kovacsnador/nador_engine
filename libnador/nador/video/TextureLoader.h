#ifndef __TEXTURE_LOADER_H__
#define __TEXTURE_LOADER_H__

#include <memory>

#include "nador/utils/Types.h"
#include "nador/system/IFileController.h"
#include "nador/log/Log.h"

namespace nador
{
    template <typename LoadStrategyTy>
    struct TextureData
    {
        /*!
         * TextureData constructor.
         *
         * \param data	The texture data.
         */
        TextureData(const FileData& data)
        {
            NADOR_ASSERT(data.IsValid());

            localBuffer = _loadStrategy.Load(reinterpret_cast<const uint8_t*>(data.GetData()), data.GetSize(), &width, &height, &pbb, 4);
        }

        /*!
         * TextureData destructor.
         */
        ~TextureData() { _loadStrategy.Free(localBuffer); }

        uint8_t* localBuffer { nullptr };
        int32_t  width { 0 };
        int32_t  height { 0 };
        int32_t  pbb { 0 }; // channels_in_file

    private:
        LoadStrategyTy _loadStrategy;
    };

    class TextureLoader
    {
    public:
        /*!
         * Load a texture from buffer.
         *
         * \param data	The texture buffer.
         *
         * \return		The raw texture data.
         */
        template <typename LoadStrategyTy>
        static auto LoadFromBuffer(const FileData& data)
        {
            return std::make_unique<TextureData<LoadStrategyTy>>(data);
        }
    };
} // namespace nador

#endif // !__TEXTURE_LOADER_H__
