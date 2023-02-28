#ifndef __UI_TEXT_LABEL_H__
#define __UI_TEXT_LABEL_H__

#include "nador/video/font/Font.h"
#include "nador/ui/IUiElement.h"

namespace nador
{
	class UiTextLabel : public IUiElement
	{
	public:
		UiTextLabel(const glm::ivec2& position,
			const FontPtr& font,
			std::string_view text,
			uint32_t fontSize,
			UiAlignment aligner = UiAlignment(),
			IUiElement* parent = nullptr);

		UiTextLabel(const glm::ivec2& position,
					const FontPtr& font,
					std::string_view text,
					UiAlignment aligner = UiAlignment(),
					const glm::vec3& fontScale = { 20, 20, 1 },
					IUiElement* parent = nullptr);

		void SetFont(const FontPtr& font);
		void SetText(std::string_view text);

		void SetColor(const glm::vec4& color) { _material.uColor = color; }
		void SetFontScale(const glm::vec3& scale) { _fontScale = scale; }

		void SetFontSize(uint32_t fontSize) { SetFontScale({ fontSize, fontSize, 1 }); };

		const FontPtr GetFont() const { return _font; };
		const std::string& GetText() const { return _text; };

		void AppendChar(uint32_t unicode);
		void AppendChar(const std::string& text);
		void RemoveLastChar();

		const glm::vec4& GetColor() const { return _material.uColor; }
		const glm::vec3& GetFontScale() const { return _fontScale; }

		int32_t GetTextHeight() const noexcept;
		int32_t GetTextWidth() const noexcept;

	protected:
		void OnRender(IRenderer* renderer, const quadVertices_t& vertices) override;

		void Update();

	private:
		glm::vec2 _CalculateSize(int32_t lineCount) const;

		FontPtr		_font;
		std::string _text;
		int32_t		_lineCount;

		BatchTextMaterial _material;

		RenderData	_renderData;
		RenderData	_renderDataBackup;

		glm::mat4	_lastModelMatrix;

		glm::vec3 _fontScale;
	};
	CREATE_PTR_TYPES(UiTextLabel);
}

#endif // !__UI_TEXT_LABEL_H__
