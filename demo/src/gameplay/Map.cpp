#include "Map.h"

namespace demo
{
    const std::vector<GameElement>& Map::GetMap() const noexcept
    {
        return _map;
    }

} // namespace demo
