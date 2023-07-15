#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <memory>
#include <vector>

#include "nador/video/IVideo.h"
#include "nador/video/renderer/IRenderer.h"
#include "nador/video/material/Material.h"
#include "nador/video/renderer/RenderData.h"
#include "nador/utils/IntervalCounter.h"

namespace nador
{
    // IntervalCounter for draw calls
    class Renderer : public IRenderer, public IntervalCounter<60>
    {
    public:
        /*!
         * Renderer constructor.
         */
        Renderer(const IVideoPtr video, rendererPlugins_t& renderers, std::unique_ptr<Camera> camera);

        /*!
         * The render begin function.
         */
        void Begin() override;

        /*!
         * The render function.
         */
        void Flush() override;

        /*!
         * The render end function.
         */
        void End() override;

        void Draw(const IMaterial* pMaterial, const RenderData& renderData, const glm::mat4& modelMatrix = IDENTITY_MATRIX) override;

        /*!
         * Gets the screen size.
         *
         * \return 		The screen size.
         */
        const glm::ivec2& GetScreenSize() const override;

        float_t GetRenderPerInterval(float_t interval = 1.f) const noexcept override;

        // takes the left bottom corner
        void SetScissor(const glm::ivec2& position, const glm::ivec2& size) const override;
        void DisableScissor() const override;

        uint32_t GetDrawCount() const noexcept override;

        void    SetCamera(std::unique_ptr<Camera> camera) noexcept override;
        Camera* GetCamera() const noexcept override;

    private:
        /*!
         * Sets the model matrix.
         *
         * \param modelMatrix		The new projection matrix.
         */
        void _SetModelMatrix(const glm::mat4& modelMatrix);

        void _SwitchRendererIfNecessary(IRenderPlugin* nextRenderer);

        const IVideoPtr _video;

        IRenderPlugin* _currentActiveRenderer { nullptr };

        rendererPlugins_t _attachedRenderers;

        std::unique_ptr<Camera> _camera;
    };

    CREATE_PTR_TYPES(Renderer);
} // namespace nador

#endif // !__RENDERER_H__
