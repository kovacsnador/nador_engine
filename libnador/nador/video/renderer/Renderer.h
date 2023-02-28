#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <memory>
#include <vector>

#include "nador/video/IVideo.h"
#include "nador/video/renderer/IRenderer.h"
#include "nador/video/shader/ShaderController.h"
#include "nador/video/buffers/VertexBuffer.h"
#include "nador/video/buffers/IndexBuffer.h"
#include "nador/video/material/Material.h"
#include "nador/video/renderer/RenderData.h"
#include "nador/utils/IntervalCounter.h"
#include "nador/video/renderer/batch_renderer/BatchRenderer.h"
#include "nador/video/renderer/simple_renderer/SimpleRenderer.h"

namespace nador
{
	//IntervalCounter for draw calls
	class Renderer : public IRenderer, public IntervalCounter<60>
	{
	public:
		/*!
		 * Renderer constructor.
		 */
		Renderer(const IVideo* video);

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

		void Draw(const IMaterial* pMaterial,
				  const RenderData& renderData,
				  const glm::mat4* modelMatrix = nullptr,
				  const glm::mat4* projectionMatrix = nullptr,
				  const glm::mat4* cameraMatrix = nullptr) override;

		/*!
		 * Gets the camera matrix.
		 *
		 * \return 		The camera matrix.
		 */
		const glm::mat4& GetCameraMatrix() const override;

		/*!
		 * Gets the projection matrix.
		 *
		 * \return 		The projection matrix.
		 */
		const glm::mat4& GetProjectionMatrix() const override;

		/*!
		 * Gets the model matrix.
		 *
		 * \return 		The model matrix.
		 */
		const glm::mat4& GetModelMatrix() const override;

		/*!
		 * Gets the screen size.
		 *
		 * \return 		The screen size.
		 */
		const glm::ivec2& GetScreenSize() const override;

		/*!
		 * Gets the current material.
		 *
		 * \return 		The current material pointer.
		 */
		/*const IMaterialPtr GetCurrentMaterial() const  override
		{
			return _currentMaterial;
		}*/

		float_t GetRenderPerInterval(float_t interval = 1.f) const override;

		// takes the left bottom corner
		virtual void SetScissor(const glm::ivec2& position, const glm::ivec2& size) const override;
		virtual void DisableScissor() const override;

		uint32_t GetDrawCount() const noexcept override;

	private:
		/*!
		 * Sets the camera matrix.
		 *
		 * \param cameraMatrix	The cameraMatrix.
		 *	Use glm::lookAt(cameraPosition, // the position of your camera, in world space
		 *					cameraTarget,   // where you want to look at, in world space
		 *					upVector		// probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down,
		 *									which can be great too
		 *					)
		 */
		void SetCameraMatrix(const glm::mat4* cameraMatrix);

		/*!
		 * Sets the projection matrix.
		 *
		 * \param projectionMatrix		The new projection matrix.
		 */
		void SetProjectionMatrix(const glm::mat4* projectionMatrix);

		/*!
		 * Sets the model matrix.
		 *
		 * \param modelMatrix		The new projection matrix.
		 */
		void SetModelMatrix(const glm::mat4* modelMatrix);

		void _SwitchRendererIfNecessary(ISimpleRenderer* nextRenderer);

		const IVideo*		_video;
		ShaderController	_shaderCtrl;

		SimpleRendererUPtr _simpleRendererPtr;
		BatchRendererUPtr  _batchRendererPtr;
		ISimpleRenderer*   _currentActiveRenderer{ nullptr };

		glm::mat4 _cameraMatrix;
		glm::mat4 _projectionMatrix;
		glm::mat4 _modelMatrix;
	};

	CREATE_PTR_TYPES(Renderer);
}

#endif // !__RENDERER_H__

