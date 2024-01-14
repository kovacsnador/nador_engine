#ifndef __NADOR_DEMO_MAP_PARSER_H__
#define __NADOR_DEMO_MAP_PARSER_H__

#include <vector>

#include "gameplay/Entity.h"
#include "nador/system/file/FileData.h"

namespace demo
{
    class MapParser
    {
    public:
        MapParser(const nador::FileData& fileData, uint32_t tileSize = 40);

        const std::vector<GameElement>& GetMap() const noexcept;
        const glm::ivec2& GetMarioStartPosition() const noexcept;

    private:
        uint32_t _tileSize;
        std::vector<GameElement> _map;
        glm::ivec2 _marioStartPosition;
    };

} // namespace demo

#endif //!__NADOR_DEMO_MAP_PARSER_H__