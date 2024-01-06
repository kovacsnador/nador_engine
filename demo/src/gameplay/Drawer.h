#ifndef __NADOR_DEMO_DRAWER_H__
#define __NADOR_DEMO_DRAWER_H__

#include <algorithm>
#include <cassert>

#include "nador/video/material/Material.h"
#include "nador/ui/UiCommon.h"
#include "nador/video/renderer/IRenderer.h"
#include "nador/video/atlas/IAtlasController.h"

#include "Entity.h"

namespace demo
{
    class Drawer
    {
    public:
        Drawer(const nador::IAtlasController* atlasCtrl)
        : _atlasCtrl(atlasCtrl)
        {
            assert(_atlasCtrl);

            _renderData.SetSquareIndices();
        }

        void SetTileSize(glm::ivec2 size)
        {
            NADOR_ASSERT(size.x > 0);
            NADOR_ASSERT(size.y > 0);
            
            _size = size; 
        }

        template<size_t N>
        void operator()(nador::IRenderer* renderer, const Entity<N>& entity)
        {
            const auto& screenSize = renderer->GetScreenSize();
            auto screenVertices = nador::GenerateVertices(screenSize);
            std::for_each(std::begin(entity.tiles), std::end(entity.tiles), [this, renderer, &screenVertices](const auto& tile){ _Draw(renderer, screenVertices, tile); });
        }


    private:
        void _Draw(nador::IRenderer* renderer, const nador::quadVertices_t& screenVertices, const Tile& tile)
        {
            auto vertices = _alignment.GenerateVertices(tile.pos, _size, screenVertices);
			auto imageData = _atlasCtrl->GetImage(tile.imageName);

            _material.texture = imageData.texture;
            _renderData.SetTextureCoords(imageData.image->uvs);
            _renderData.SetVertices(vertices);

            renderer->Draw(&_material, _renderData, nador::IDENTITY_MATRIX);
        }

        const nador::IAtlasController* _atlasCtrl;

        nador::UiAlignment _alignment;
        nador::BatchMaterial _material;
        nador::RenderData _renderData;

        glm::ivec2 _size {40, 40};
    };
} // namespace demo

#endif //!__NADOR_DEMO_DRAWER_H__