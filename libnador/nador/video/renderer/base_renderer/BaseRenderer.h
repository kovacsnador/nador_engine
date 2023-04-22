#ifndef __NADOR_SIMPLE_REDNERER_H__
#define __NADOR_SIMPLE_REDNERER_H__

#include "nador/video/renderer/IRenderPlugin.h"
#include "nador/video/material/Material.h"
#include "nador/video/renderer/RenderData.h"
#include "nador/video/shader/IShaderController.h"

namespace nador
{
    class BaseRenderer : public IRenderPlugin
    {
    public:
        BaseRenderer(const IVideoPtr video, IShaderControllerPtr shaderCtrl, size_t maxVertexCount);

        void Begin() override;
        void End() override;
        bool Flush() override;

        void Draw(const IMaterial* material, const RenderData& renderData, const glm::mat4& uMVP) override;

    private:
        bool _SetMaterial(const IMaterial* material);
        void _AddRenderData(const RenderData& renderData);
        void _ResetBuffers();

        bool _IsSameMaterial(const IMaterial* material);

        const IVideoPtr         _video;
        RenderData            _renderData;
        IMaterialPtr          _currentMaterial;
        ShaderPtr             _currentShader;
        IShaderControllerPtr  _shaderCtrl;

        glm::mat4 _uMVP;

        size_t _maxVertexCount;
    };
    CREATE_PTR_TYPES(BaseRenderer);
} // namespace nador

#endif // !__NADOR_SIMPLE_REDNERER_H__
