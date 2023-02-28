#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <memory>

#include "nador/video/shader/Shader.h"
#include "nador/video/buffers/Texture.h"
#include "nador/video/buffers/VertexBuffer.h"

namespace nador
{
	class IMaterial;
	CREATE_PTR_TYPES(IMaterial);


	class IMaterial
	{
	public:
		/*!
		 * Virtual destructor.
		 */
		virtual ~IMaterial() = default;

		/*!
		 * Activate material in shader program.
		 *
		 * \param shader	The current shader program. 
		 */
		virtual void Activate(const ShaderPtr& shader, const glm::mat4& uMVP) const = 0;

		/*!
		 * Compare two materials.
		 *
		 * \param material		To compare with material
		 *
		 * \return		True if they are same, false otherwise.
		 */
		virtual bool IsSame(const IMaterial* material) const = 0;

		/*!
		 * Get the shader type he needs.
		 *
		 * \return		The shader type.
		 */
		virtual EShader ShaderTypeNeeded() const noexcept = 0;

		/*!
		 * Assign a material to another.
		 *
		 * \param material		The material to assign.
		 */
		virtual void Assign(const IMaterial* material) = 0;

		virtual const char* GetVerticesName() { return "aPosition"; };
		virtual const char* GetTextureCoordName() { return "aTexCoord"; };

		virtual IMaterialPtr Clone() const = 0;
	};

	//==========================================================================
	class BaseMaterial : public IMaterial
	{
	public:
		/*!
		 * Activate material in shader program.
		 *
		 * \param shader	The current shader program.
		 */
		void Activate(const ShaderPtr& shader, const glm::mat4& uMVP) const override;

		/*!
		 * Compare two materials.
		 *
		 * \param material		To compare with material
		 *
		 * \return		True if they are same, false otherwise.
		 */
		bool IsSame(const IMaterial* material) const override;

		/*!
		 * Get the shader type he needs.
		 *
		 * \return		The shader type.
		 */
		EShader ShaderTypeNeeded() const noexcept override;

		/*!
		 * Assign a material to another.
		 *
		 * \param material		The material to assign.
		 */
		void Assign(const IMaterial* material) override;

		IMaterialPtr Clone() const override;

		glm::vec4 uColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	//==========================================================================
	class TextureMaterial : public IMaterial
	{
	public:
		/*!
		 * Activate material in shader program.
		 *
		 * \param shader	The current shader program.
		 */
		void Activate(const ShaderPtr& shader, const glm::mat4& uMVP) const override;

		/*!
		 * Compare two materials.
		 *
		 * \param material		To compare with material
		 *
		 * \return		True if they are same, false otherwise.
		 */
		bool IsSame(const IMaterial* material) const override;

		/*!
		 * Get the shader type he needs.
		 *
		 * \return		The shader type.
		 */
		EShader ShaderTypeNeeded() const noexcept override;

		/*!
		 * Assign a material to another.
		 *
		 * \param material		The material to assign.
		 */
		void Assign(const IMaterial* material) override;

		IMaterialPtr Clone() const override;

		TexturePtr	texture;
		glm::vec4	uColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	//==========================================================================
	class FontMaterial : public TextureMaterial
	{
	public:
		/*!
		 * Get the shader type he needs.
		 *
		 * \return		The shader type.
		 */
		EShader ShaderTypeNeeded() const noexcept override;

		/*!
		 * Compare two materials.
		 *
		 * \param material		To compare with material
		 *
		 * \return		True if they are same, false otherwise.
		 */
		bool IsSame(const IMaterial* material) const override;

		IMaterialPtr Clone() const override;
	};

	//==========================================================================
	class RoundEdgeMaterial : public IMaterial
	{
	public:
		void Activate(const ShaderPtr& shader, const glm::mat4& uMVP) const override;
		EShader ShaderTypeNeeded() const noexcept override;
		bool IsSame(const IMaterial* material) const override;
		void Assign(const IMaterial* material) override;

		IMaterialPtr Clone() const override;

		glm::vec4	uColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec2	uDimensions{ 0,0 };
		float_t		uRadius{ 0 };
	};

	class BatchMaterial : public IMaterial
	{
	public:
		void Activate(const ShaderPtr& shader, const glm::mat4& uMVP) const override;
		EShader ShaderTypeNeeded() const noexcept override;
		bool IsSame(const IMaterial* material) const override;
		void Assign(const IMaterial* material) override;

		virtual bool IsText() const noexcept;

		IMaterialPtr Clone() const override;

		TexturePtr	texture;
		glm::vec4	uColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class BatchTextMaterial : public BatchMaterial
	{
	public:
		bool IsText() const noexcept override;
	};
}

#endif // !__MATERIAL_H__

