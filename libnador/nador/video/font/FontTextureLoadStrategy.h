#ifndef __NADOR_FONT_TEXTURE_LOAD_STRATEGY_H__
#define __NADOR_FONT_TEXTURE_LOAD_STRATEGY_H__

#include <functional>
#include <memory>

namespace nador
{
    namespace font
    {
        template <typename T>
        using FontTextureLoadStrategy_t = std::function<std::unique_ptr<T>(uint32_t, uint32_t)>;

    } // namespace font

} // namespace nador

#endif //!__NADOR_FONT_TEXTURE_LOAD_STRATEGY_H__