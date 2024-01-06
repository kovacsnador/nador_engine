#ifndef __NADOR_DEMO_MAP_H__
#define __NADOR_DEMO_MAP_H__

#include "Entity.h"

namespace demo
{
    class Map
    {
    public:
        Map(std::vector<GameElement> map)
        : _map(std::move(map))
        {
        }

        const std::vector<GameElement>& GetMap() const noexcept;

    private:
        std::vector<GameElement> _map;
    };
} // namespace demo


#endif //!__NADOR_DEMO_MAP_H__