#include <iostream>

#include "nador/utils/Utils.h"

#include "MapParser.h"

namespace demo
{
    MapParser::MapParser(const nador::FileData& fileData, uint32_t tileSize)
    : _tileSize(tileSize)
    {
        nador::utils::IterableWrapper iw(fileData, [](const auto& v){ return v.Begin(); }, [](const auto& v){ return v.End(); });

        glm::ivec2 currentPosition{0,0};

        auto rightShift = [this](glm::ivec2& pos, int32_t count = 1){ pos.x += _tileSize * count; };
        auto downShift = [this](glm::ivec2& pos){ pos.x = 0; pos.y += _tileSize; };
        
        for(auto c : iw)
        {
            switch (c)
            {
            case '\n':
                downShift(currentPosition);
                break;
            case '\t':
                rightShift(currentPosition, 4);
                break;
            case ' ':
                rightShift(currentPosition);
                break;
            case 'G':   // ground
                _map.push_back(Entity<1>{currentPosition, {tileSize, tileSize}, nador::video::EImageName::BLOCKROW_1_COLUMN_5});
                rightShift(currentPosition);
                break;
            case 'B':   // block
                _map.push_back(Entity<1>{currentPosition, {tileSize, tileSize}, nador::video::EImageName::BLOCKROW_1_COLUMN_2});
                rightShift(currentPosition);
                break; 
            case 'C':   // coin
                _map.push_back(Entity<1>{currentPosition, {tileSize, tileSize}, nador::video::EImageName::BLOCKROW_2_COLUMN_10});
                rightShift(currentPosition); 
                break;
            case 'M':   // mario position
                _marioStartPosition = currentPosition;
                rightShift(currentPosition);
                break;            
            default:
                break;
            }
        }  
    }

    const std::vector<GameElement>& MapParser::GetMap() const noexcept
    {
        return _map;
    }

    const glm::ivec2 & MapParser::GetMarioStartPosition() const noexcept
    {
        return _marioStartPosition;
    }

} // namespace demo
