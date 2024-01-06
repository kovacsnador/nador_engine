#include <iostream>

#include "nador/utils/Utils.h"

#include "MapParser.h"

namespace demo
{
    MapParser::MapParser(const nador::FileData& fileData)
    {
        nador::utils::IterableWrapper iw(fileData, [](const auto& v){ return v.Begin(); }, [](const auto& v){ return v.End(); });

        for(auto c : iw)
        {
            std::cout << c;
        }  
    }

} // namespace demo
