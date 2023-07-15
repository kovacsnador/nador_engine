#include <algorithm>

#include "nador/video/renderer/Renderer.h"
#include "nador/log/Log.h"
#include "Renderer.h"

static constexpr size_t MAX_VERTEX_COUNT = 10000;

namespace nador
{
    Renderer::Renderer(const IVideoPtr video, rendererPlugins_t& renderers, std::unique_ptr<Camera> camera)
    : _video(std::move(video))
    , _attachedRenderers(std::move(renderers))
    , _camera(std::move(camera))
    {
        NADOR_ASSERT(_video);
        
        _video->EnableBlend();
        _video->EnableMultiSample();
    }

    void Renderer::Begin()
    {
        _video->ClearColorRGBA(0, 0, 0, 1);

        for (auto& [_, value] : _attachedRenderers)
        {
            value->Begin();
        }
    }

    void Renderer::Flush()
    {
        if (_currentActiveRenderer)
        {
            _currentActiveRenderer->Flush();
        }
    }

    void Renderer::End()
    {
        for (auto& [_, value] : _attachedRenderers)
        {
            value->End();
        }
    }

    void Renderer::Draw(const IMaterial*  pMaterial,
                        const RenderData& renderData,
                        const glm::mat4&  modelMatrix)
    {
        ERenderPlugin rendererPlugin = pMaterial->GetRenderPlugin();

        auto renderer = _attachedRenderers.find(rendererPlugin);

        if (renderer == _attachedRenderers.end())
        {
            ENGINE_ERROR("RenderPlugin not attached %d", rendererPlugin);
            return;
        }

        _SwitchRendererIfNecessary(renderer->second.get());

        glm::mat4 MVPmatrix = _camera->GetCameraMtx() * modelMatrix;
        _currentActiveRenderer->Draw(pMaterial, renderData, MVPmatrix);

        // count up draw count
        CountUp();
    }

    const glm::ivec2& Renderer::GetScreenSize() const
    {
        return _video->GetScreenSize();
    }

    float_t Renderer::GetRenderPerInterval(float_t interval) const noexcept
    {
        return GetCountPerInterval(interval);
    }

    uint32_t Renderer::GetDrawCount() const noexcept
    {
        uint32_t drawCount = 0;
        for (auto& [key, value] : _attachedRenderers)
        {
            drawCount += value->GetDrawCount();
        }
        return drawCount;
    }

    void Renderer::SetCamera(std::unique_ptr<Camera> camera) noexcept
    {
        _camera = std::move(camera);
    }

    Camera* Renderer::GetCamera() const noexcept
    {
        return _camera.get();
    }

    void Renderer::SetScissor(const glm::ivec2& position, const glm::ivec2& size) const
    {
        _video->SetScissor(position, size);
    }

    void Renderer::DisableScissor() const
    {
        _video->DisableScissor();
    }

    void Renderer::_SwitchRendererIfNecessary(IRenderPlugin* nextRenderer)
    {
        NADOR_ASSERT(nextRenderer);

        if (_currentActiveRenderer != nextRenderer)
        {
            // switching renderer
            if (_currentActiveRenderer)
            {
                _currentActiveRenderer->Flush();
            }
            _currentActiveRenderer = nextRenderer;
        }
    }
} // namespace nador