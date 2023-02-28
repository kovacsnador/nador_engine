#include "imgui.h"

#include <optional>
#include <utility>

#include "nador/test/test_views/FontTest.h"
#include "nador/App.h"
#include "nador/utils/ImGuiHelper.h"


namespace nador
{
	template< class T, class U >
	constexpr bool cmp_less( T t, U u ) noexcept
	{
    	using UT = std::make_unsigned_t<T>;
    	using UU = std::make_unsigned_t<U>;
    	if constexpr (std::is_signed_v<T> == std::is_signed_v<U>)
        	return t < u;
    	else if constexpr (std::is_signed_v<T>)
        	return t < 0 ? true : UT(t) < u;
    	else
        	return u < 0 ? false : t < UU(u);
	}

	static constexpr size_t INPUT_TEXT_BUFFER_SIZE = 5000;

	static std::optional<uint32_t> SelectedFontTypeMap(int32_t selectedFontType, const strings_t& fontTypes, const IFontController* fontCtrl)
	{
		if(cmp_less(selectedFontType, fontTypes.size()))
		{
			return fontCtrl->GetFontIdByName(fontTypes[selectedFontType]);
		}

		return std::nullopt;
	}

	static std::optional<uint32_t> SelectedFontSizeMap(int32_t selectedFontSize, const strings_t& fontSizes)
	{
		if(cmp_less(selectedFontSize, fontSizes.size()))
		{
			return std::stoul(fontSizes[selectedFontSize]);
		}

		return std::nullopt;
	}

	FontTest::FontTest()
	: _position(2, 500)
	, _scale(20, 20, 1)
	, _color(1, 0, 0, 1)
	{
		_inputTextBuffer = std::make_unique<char[]>(INPUT_TEXT_BUFFER_SIZE);
		std::memset(_inputTextBuffer.get(), 0, INPUT_TEXT_BUFFER_SIZE);
	}

	void FontTest::OnRender(IRenderer* renderer)
	{
		if(_fontMaterial.texture == nullptr)
		{
			return;
		}

		glm::mat4 projMultiCamera = renderer->GetProjectionMatrix() * renderer->GetCameraMatrix();
		glm::vec3 worldPosition = utils::ScreenToWorldPosition(_position, renderer->GetScreenSize(), projMultiCamera);

		worldPosition.z = 0;

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), worldPosition);

		glm::mat4 scaleMatrix = glm::scale(_scale);

		modelMatrix *= scaleMatrix;

		renderer->Draw(&_fontMaterial, _renderData, &modelMatrix);
	}

	void FontTest::OnDebugRender()
	{
		IFontController* fontCtrl = IApp::Get()->GetFontController();

		strings_t fontTypeNames = fontCtrl->GetRegisteredFontNames();
		strings_t fontSizeNames = fontCtrl->GetFontSizesAsString();

		static int32_t selectedFontType = 0;
		static int32_t selectedFontSize = 0;

		bool reloadNeeds = false;

		reloadNeeds |= ImGui::Button("Reload");

		reloadNeeds |= ImGui::InputTextMultiline("Text", _inputTextBuffer.get(), INPUT_TEXT_BUFFER_SIZE);

		reloadNeeds |= ImGui::ListBox("FontTypes", &selectedFontType,
					   ImGuiStrContainerIter,
					   reinterpret_cast<void*>(&fontTypeNames), fontTypeNames.size(), 3);

		reloadNeeds |= ImGui::ListBox("FontSize", &selectedFontSize,
					   ImGuiStrContainerIter,
					   reinterpret_cast<void*>(&fontSizeNames), fontSizeNames.size(), 3);

		if(reloadNeeds)
		{
			std::optional<uint32_t> fontType = SelectedFontTypeMap(selectedFontType, fontTypeNames, fontCtrl);
			if(fontType.has_value() == false)
			{
				ENGINE_WARNING("FontType has no value. (possible not implemented)");
			}

			std::optional<uint32_t> fontSize = SelectedFontSizeMap(selectedFontSize, fontSizeNames);
			if(fontSize.has_value() == false)
			{
				ENGINE_WARNING("FontSize has no value. (possible not implemented)");
			}

			if(fontType.has_value() && fontSize.has_value())
			{
				_LoadFont(fontType.value(), fontSize.value());
			}
		}

		ImGui::SliderFloat3("Text scale", &_scale.x, 1.0f, 100.0f);
		ImGui::SliderFloat2("Text position", &_position.x, 1.0f, 1000.0f);

		if(ImGui::ColorEdit4("Text Color", &_color.r))
		{
			_fontMaterial.uColor = _color;
		}
	}

	void FontTest::_LoadFont(uint32_t fontId, uint32_t fontSize)
	{
		const IFontController* fontCtrl = IApp::Get()->GetFontController();
		const FontPtr font = fontCtrl->GetFont(fontId, fontSize);

		if(font == nullptr)
		{
			ENGINE_WARNING("Font test is not prepared!");
			return;
		}

		_renderData = font->CalculateUTF8Text(_inputTextBuffer.get());

		_fontMaterial.texture = font->GetTexture();
		_fontMaterial.uColor = _color;
	}
} 