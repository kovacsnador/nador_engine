#include "nador/video/material/Material.h"
#include "Material.h"

namespace nador
{
    void BaseMaterial::Activate(const ShaderPtr& shader, const glm::mat4& uMVP) const
    {
        shader->Bind();
        shader->SetUniform4f("uColor", uColor);
        shader->SetUniformMat4f("uMVP", uMVP);
    }

    bool BaseMaterial::IsSame(const IMaterial* material) const
    {
        if (const BaseMaterial* mat = dynamic_cast<const BaseMaterial*>(material))
        {
            return uColor == mat->uColor;
        }

        return false;
    }

    EShader BaseMaterial::ShaderTypeNeeded() const noexcept
    {
        return EShader::BASE;
    }

    void BaseMaterial::Assign(const IMaterial* material)
    {
        if (const BaseMaterial* mat = dynamic_cast<const BaseMaterial*>(material))
        {
            *this = *mat;
        }
    }

    IMaterialPtr BaseMaterial::Clone() const
    {
        return std::make_shared<BaseMaterial>(*this);
    }

    ERenderPlugin BaseMaterial::GetRenderPlugin() const noexcept
    {
        return ERenderPlugin::EBaseRenderer;
    }

    void TextureMaterial::Activate(const ShaderPtr& shader, const glm::mat4& uMVP) const
    {
        shader->Bind();
        shader->SetUniform4f("uColor", uColor);

        shader->SetUniform1i("uTexture", 0);
        texture->Bind();

        shader->SetUniformMat4f("uMVP", uMVP);
    }

    bool TextureMaterial::IsSame(const IMaterial* material) const
    {
        if (const TextureMaterial* mat = dynamic_cast<const TextureMaterial*>(material))
        {
            return uColor == mat->uColor && texture->GetId() == mat->texture->GetId();
        }

        return false;
    }

    EShader TextureMaterial::ShaderTypeNeeded() const noexcept
    {
        return EShader::TEXTURE;
    }

    void TextureMaterial::Assign(const IMaterial* material)
    {
        if (const TextureMaterial* mat = dynamic_cast<const TextureMaterial*>(material))
        {
            *this = *mat;
        }
    }

    IMaterialPtr TextureMaterial::Clone() const
    {
        return std::make_shared<TextureMaterial>(*this);
    }

    ERenderPlugin TextureMaterial::GetRenderPlugin() const noexcept
    {
        return ERenderPlugin::EBaseRenderer;
    }

    EShader FontMaterial::ShaderTypeNeeded() const noexcept
    {
        return EShader::FONT;
    }

    bool FontMaterial::IsSame(const IMaterial* material) const
    {
        if (const FontMaterial* mat = dynamic_cast<const FontMaterial*>(material))
        {
            return uColor == mat->uColor && texture->GetId() == mat->texture->GetId();
        }

        return false;
    }

    IMaterialPtr FontMaterial::Clone() const
    {
        return std::make_shared<FontMaterial>(*this);
    }

    ERenderPlugin FontMaterial::GetRenderPlugin() const noexcept
    {
        return ERenderPlugin::EBaseRenderer;
    }

    void RoundEdgeMaterial::Activate(const ShaderPtr& shader, const glm::mat4& uMVP) const
    {
        shader->Bind();
        shader->SetUniform4f("uColor", uColor);
        shader->SetUniform2f("uDimensions", uDimensions);
        shader->SetUniform1f("uRadius", uRadius);
        shader->SetUniformMat4f("uMVP", uMVP);
    }

    EShader RoundEdgeMaterial::ShaderTypeNeeded() const noexcept
    {
        return EShader::ROUND_EDGE;
    }

    bool RoundEdgeMaterial::IsSame(const IMaterial* material) const
    {
        if (const RoundEdgeMaterial* mat = dynamic_cast<const RoundEdgeMaterial*>(material))
        {
            return (uColor == mat->uColor && uDimensions == mat->uDimensions && uRadius == mat->uRadius);
        }

        return false;
    }

    void RoundEdgeMaterial::Assign(const IMaterial* material)
    {
        if (const RoundEdgeMaterial* mat = dynamic_cast<const RoundEdgeMaterial*>(material))
        {
            *this = *mat;
        }
    }

    IMaterialPtr RoundEdgeMaterial::Clone() const
    {
        return std::make_shared<RoundEdgeMaterial>(*this);
    }

    ERenderPlugin RoundEdgeMaterial::GetRenderPlugin() const noexcept
    {
        return ERenderPlugin::EBaseRenderer;
    }

    void BatchMaterial::Activate(const ShaderPtr& shader, const glm::mat4& uMVP) const
    {
        shader->Bind();
        shader->SetUniformMat4f("uMVP", uMVP);
    }

    EShader BatchMaterial::ShaderTypeNeeded() const noexcept
    {
        return EShader::BATCH;
    }

    bool BatchMaterial::IsSame(const IMaterial* material) const
    {
        return dynamic_cast<const BatchMaterial*>(material);
    }

    void BatchMaterial::Assign(const IMaterial* material)
    {
        if (const BatchMaterial* mat = dynamic_cast<const BatchMaterial*>(material))
        {
            *this = *mat;
        }
    }

    bool BatchMaterial::IsText() const noexcept
    {
        return false;
    }

    IMaterialPtr BatchMaterial::Clone() const
    {
        return std::make_shared<BatchMaterial>(*this);
    }

    ERenderPlugin BatchMaterial::GetRenderPlugin() const noexcept
    {
        return ERenderPlugin::EBatchRenderer;
    }

    bool BatchTextMaterial::IsText() const noexcept
    {
        return true;
    }
} // namespace nador