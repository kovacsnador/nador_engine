#include <codecvt>

#include "nador/ui/UiTextLabel.h"
#include "nador/utils/Utils.h"

namespace nador
{
	static constexpr float_t EXTRA_BB_SPACE = 4;

	UiTextLabel::UiTextLabel(const glm::ivec2& position,
							const FontPtr& font,
							std::string_view text,
							uint32_t fontSize,
							UiAlignment aligner,
							IUiElement* parent)
	: IUiElement(position, { 0, 0 }, aligner, parent)
	, _font(font)
	, _text(text)
	, _lineCount(1)
	, _lastModelMatrix(1.0f)
	{
		SetFontSize(fontSize);
		Update();
	}

	UiTextLabel::UiTextLabel(const glm::ivec2& position,
							 const FontPtr& font,
							 std::string_view text,
							 UiAlignment aligner,
							 const glm::vec3& scale,
							 IUiElement* parent)
	: IUiElement(position, {0, 0}, aligner, parent)
	, _font(font)
	, _text(text)
	, _lineCount(1)
	, _lastModelMatrix(1.0f)
	, _fontScale(scale)
	{
		Update();
	}

	void UiTextLabel::SetFont(const FontPtr& font)
	{
		NADOR_ASSERT(font);

		_font = font;
		Update();
	}

	void UiTextLabel::SetText(std::string_view text)
	{
		_text = text;
		Update();
	}

	void UiTextLabel::AppendChar(uint32_t unicode)
	{
		wchar_t txt[] = {'\0', '\0', '\0', '\0'};
		memset(txt, 0, sizeof(txt));
		memcpy(txt, &unicode, sizeof(unicode));

		std::wstring ws(txt);

		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;

		_text += converter.to_bytes(ws);
		
		Update();
	}

	void UiTextLabel::AppendChar(const std::string& text)
	{
		_text += text;
		
		Update();
	}

	void UiTextLabel::RemoveLastChar()
	{
		utils::PopBackUtf8(_text);
		Update();
	}

	void UiTextLabel::OnRender(IRenderer* renderer, const quadVertices_t& vertices)
	{
		if (_renderData.vertices.empty())
		{
			return;
		}

		auto size = _CalculateSize(_lineCount);

		auto yTextOffset = size.y * 0.85f / _lineCount;

		// Bottom left corner for text orientation
		glm::vec3 position{ vertices[3].x, vertices[3].y - yTextOffset + (EXTRA_BB_SPACE / 2), vertices[3].z };

		glm::mat4 projMultiCamera = renderer->GetProjectionMatrix() * renderer->GetCameraMatrix();
		glm::vec3 worldPosition = utils::ScreenToWorldPosition(position, renderer->GetScreenSize(), projMultiCamera);

		glm::mat4 modelMatrix = glm::translate(IDENTITY_MATRIX, worldPosition);

		glm::mat4 scaleMatrix = glm::scale(_fontScale * _scale);

		modelMatrix *= scaleMatrix;

		if (modelMatrix != _lastModelMatrix)
		{
			_renderData.vertices = _renderDataBackup.vertices;

			VerticesMultiMatrix(_renderData.vertices, modelMatrix);

			_lastModelMatrix = modelMatrix;
		}

		renderer->Draw(&_material, _renderData, &IDENTITY_MATRIX);
	}

	void UiTextLabel::Update()
	{
		NADOR_ASSERT(_font);

		_renderData = _font->CalculateUTF8Text(_text.c_str());
		_renderDataBackup = _renderData;
		_material.texture = _font->GetTexture();
		
		// counting Lines in text
		std::regex regex("(\r\n|\r|\n)");
		int32_t lineCount = utils::CountMatchInRegex(_text, regex) + 1;
		_lineCount = std::max(1, lineCount);

		// Updating the element size
		auto size = _CalculateSize(_lineCount);
		SetSize(size);

		// force to update for the next OnRender call
		_lastModelMatrix = IDENTITY_MATRIX;
	}

	glm::vec2 UiTextLabel::_CalculateSize(int32_t lineCount) const
	{
		glm::vec2 size = (_renderDataBackup.GetBoundingBox() * glm::vec2(_fontScale)) + glm::vec2(EXTRA_BB_SPACE, EXTRA_BB_SPACE);
		if (lineCount == 1)
		{
			size.y = (_font->GetMaxHeight() * _fontScale.y + EXTRA_BB_SPACE) * lineCount;
		}
		return size;
	}

	int32_t UiTextLabel::GetTextHeight() const noexcept
	{
		NADOR_ASSERT(_font);

		return static_cast<int32_t>(_font->GetMaxHeight() * _fontScale.y);
	}

	int32_t UiTextLabel::GetTextWidth() const noexcept
	{
		return _size.x;
	}
}