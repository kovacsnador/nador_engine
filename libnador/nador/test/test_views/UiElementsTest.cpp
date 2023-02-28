#include "imgui.h"

#include "nador/App.h"
#include "nador/test/test_views/UiElementsTest.h"
#include "nador/test/TestCommon.h"

namespace nador
{
	static constexpr const char* LOREM_IPSUM = R"(Lorem ipsum dolor sit amet, consetetur sadipscing elitr,
												sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat,
												sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum.
												Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
												Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor
												invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam 
												et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem
												ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy
												eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam
												et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
												Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla
												facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te
												feugait nulla facilisi.Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut
												laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis
												nisl ut aliquip ex ea commodo consequat.Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat,
												vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril
												delenit augue duis dolore te feugait nulla facilisi. Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet
												doming id quod mazim placerat facer)";


	UiElementsTest::UiElementsTest()
	{
		_uiImage.reset(new UiImage({ 0, 200 }, { 100, 100 }, video::EImageName::BACK));
		_uiImage->SetName("UiImage");

		auto onMousePressedCallback = [this](EMouseButton mouseButton, const glm::vec2&, IUiElement* elem)->bool {
			SelectElement(elem, elem->GetName());

			if (mouseButton == EMouseButton::LEFT)
			{
				_selectedDragMode = true;
			}

			return true; };


		_uiImage->SetOnMousePressedCallback(onMousePressedCallback);

		const IFontController* fontCtrl = IApp::Get()->GetFontController();
		FontPtr font = fontCtrl->GetDefaultSystemFont();

		if (font)
		{
			_uiTextLabelMultiLine.reset(new UiTextLabel({ 0, 300 }, font, LOREM_IPSUM));
			_uiTextLabelMultiLine->SetName("UiTextLabelMultiLine");

			_uiTextLabelMultiLine->SetOnMousePressedCallback(onMousePressedCallback);

			_uiTextLabel.reset(new UiTextLabel({ 0, 400 }, font, "UiTextLabely "));
			_uiTextLabel->SetName("UiTextLabel");

			_uiTextLabel->SetOnMousePressedCallback(onMousePressedCallback);
		}
		else
		{
			ENGINE_WARNING("UiTextLabel not created in UiElementsTest");
		}

		UiButton::UiButtonImages buttonImagesWhite{ video::EImageName::FEHERMOSOLYOG, video::EImageName::FEHERDRAW, video::EImageName::FEHERSZOM };
		_uiButtonWhite.reset(new UiButton({200, 100}, {100, 100}, buttonImagesWhite));
		_uiButtonWhite->SetName("UiButtonWhite");
		_uiButtonWhite->SetOnMousePressedCallback(onMousePressedCallback);

		UiButton::UiButtonImages buttonImagesBlack{ video::EImageName::FEKETEMOSOLY, video::EImageName::FEKETEDRAW, video::EImageName::FEKETESZOM };
		_uiButtonBlack.reset(new UiButton({ 250, 150 }, { 100, 100 }, buttonImagesBlack));
		_uiButtonBlack->SetName("UiButtonBlack");

		_uiButtonBlack->SetOnMousePressedCallback(onMousePressedCallback);

		UiButton::UiButtonImages buttonImagesWithText{ video::EImageName::BLUEBUTTON, video::EImageName::BLUEBUTTON, video::EImageName::BLUEBUTTON };
		_uiButtonWithText.reset(new UiButton({ 250, 150 }, { 150, 50 }, buttonImagesWithText));
		_uiButtonWithText->SetName("UiButtonWithText");

		_uiButtonWithText->SetOnMousePressedCallback(onMousePressedCallback);

		if (font)
		{
			_uiButtonWithText->SetUiTextLabel(font, "Text\nText");
		}

		_uiRoundEdgeOverlay.reset(new UiRoundEdgeOverlay({250, 250}, {100, 250}, {1.0f, 1.0f, 0.0f, .3f }));
		_uiRoundEdgeOverlay->SetName("UiRoundEdgeOverlay");
		_uiRoundEdgeOverlay->SetEdgeRadius(15);
		_uiRoundEdgeOverlay->SetOnMousePressedCallback(onMousePressedCallback);

		_uiOverlay.reset(new UiOverlay({ 550, 550 }, { 200, 250 }, { .4f, 1.0f, .3f, .3f }));
		_uiOverlay->SetName("UiOverlay");
		_uiOverlay->SetOnMousePressedCallback(onMousePressedCallback);

		_uiScrollList.reset(new UiScrollList({ 500, 300 }, { 150, 400 }));
		_uiScrollList->SetBackground(video::EImageName::BLUEBUTTON);
		_uiScrollList->SetName("UiScrollList");
		//_uiScrollList->SetOnMousePressedCallback(onMousePressedCallback);

		auto* scrollListBackground = _uiScrollList->GetBackground();
		scrollListBackground->SetOpacity(0.1f);

		for (size_t i = 0; i < 20; ++i)
		{	
			auto btn = std::make_shared<UiButton>(glm::ivec2(0, 0), glm::ivec2(0, 30), buttonImagesWithText);

			if (font)
			{
				btn->SetUiTextLabel(font, std::to_string(i + 1));
			}
			
			_uiScrollList->AddElement(btn);
		}

		auto& aligner = _uiScrollList->GetAligner();
		aligner.SetOffset({ 0, 0 });
		aligner.SetDiff(8);
		aligner.SetAlignment({ EHorizontalAlignment::STRETCH, EVerticalAlignment::TOP });

		if (font)
		{
			UiEditText::UiEditTextImages editTextImages{ video::EImageName::RECTANGLE, video::EImageName::RECTANGLEDOWN, video::EImageName::RECTANGLEOVER };
			_uiEditText.reset(new UiEditText({ 300, 30 }, { 150, 30 }, editTextImages, font));
			_uiEditText->SetName("UiEditText");
			_uiEditText->SetOnMousePressedCallback(onMousePressedCallback);

			_uiEditTextSimple.reset(new UiEditText({ 500, 30 }, { 150, 30 }, font));
			_uiEditTextSimple->SetName("UiEditTextSimple");
			_uiEditTextSimple->SetOnMousePressedCallback(onMousePressedCallback);
		}

		_lastMousePosition = IApp::Get()->GetInputController()->GetMousePosition();
	}

	void UiElementsTest::OnDebugRender()
	{
		static const char* horizontal[]{ "LEFT", "RIGHT", "CENTER", "STRETCH"};
		static const char* vertical[]{ "TOP", "BOTTOM", "CENTER", "STRETCH" };

		IUiApp* uiApp = IApp::Get()->GetUiApp();
		auto currentMousePosition = IApp::Get()->GetInputController()->GetMousePosition();

		if (IApp::Get()->GetInputController()->IsMouseButtonReleased(EMouseButton::LEFT))
		{
			_selectedDragMode = false;
		}

		if (ImGui::Button("Toggle Debug Edge Drawing"))
		{
			uiApp->DebugDrawEdge(!uiApp->IsDebugDrawEdge());
		}

		AddButton(_uiImage.get(), EUiLayer::OVERLAY);
		AddButton(_uiTextLabel.get(), EUiLayer::OVERLAY);
		AddButton(_uiTextLabelMultiLine.get(), EUiLayer::OVERLAY);
		AddButton(_uiButtonWhite.get(), EUiLayer::OVERLAY);
		AddButton(_uiButtonBlack.get(), EUiLayer::OVERLAY);
		AddButton(_uiButtonWithText.get(), EUiLayer::OVERLAY);
		AddButton(_uiRoundEdgeOverlay.get(), EUiLayer::OVERLAY);
		AddButton(_uiOverlay.get(), EUiLayer::OVERLAY);
		AddButton(_uiScrollList.get(), EUiLayer::OVERLAY);
		AddButton(_uiEditText.get(), EUiLayer::OVERLAY);
		AddButton(_uiEditTextSimple.get(), EUiLayer::OVERLAY);

		if (_selectedUiElement)
		{
			const auto& screenSize = IApp::Get()->GetRenderer()->GetScreenSize();

			glm::ivec2 selectedPosition = _selectedUiElement->GetPosition();
			glm::ivec2 selectedOffset = _selectedUiElement->GetOffset();
			const UiAlignment& aligner = _selectedUiElement->GetAligner();

			int32_t selectedHorizontal = (int32_t)aligner.eHorizontAlign;
			int32_t selectedVertical = (int32_t)aligner.eVerticAlign;

			bool needUpdate = false;

			ImGui::NewLine();
			ImGui::Text(_selectedName.c_str());
			needUpdate |= ImGui::SliderInt2("Position X Y", &selectedPosition.x, 0, std::min(screenSize.x, screenSize.y));
			needUpdate |= ImGui::InputInt2("Offset X Y", &selectedOffset.x);
			needUpdate |= ImGui::Combo("Horizontal", &selectedHorizontal, horizontal, IM_ARRAYSIZE(horizontal));
			needUpdate |= ImGui::Combo("Vertical", &selectedVertical, vertical, IM_ARRAYSIZE(vertical));

			if (needUpdate)
			{
				_selectedUiElement->SetPosition(selectedPosition);
				_selectedUiElement->SetOffset(selectedOffset);
				_selectedUiElement->SetAlignment(ToAligner(selectedHorizontal, selectedVertical));
			}

			const auto& alignment = _selectedUiElement->GetAligner();

			if (_selectedDragMode)
			{
				glm::ivec2 diff = currentMousePosition - _lastMousePosition;
				auto pos = _selectedUiElement->GetPosition();
				if (alignment.eHorizontAlign == EHorizontalAlignment::LEFT)
				{
					pos.x += diff.x;
				}
				if (alignment.eHorizontAlign == EHorizontalAlignment::RIGHT)
				{
					pos.x -= diff.x;
				}

				if (alignment.eVerticAlign == EVerticalAlignment::TOP)
				{
					pos.y -= diff.y;
				}
				if (alignment.eVerticAlign == EVerticalAlignment::BOTTOM)
				{
					pos.y += diff.y;
				}
				_selectedUiElement->SetPosition(pos);
			}
		}

		_lastMousePosition = currentMousePosition;
	}

	void UiElementsTest::SelectElement(IUiElement* elem, std::string_view name)
	{
		_selectedUiElement = elem;
		_selectedName = name;
	}

	void UiElementsTest::SelectElementAndAddToLayer(IUiElement* elem, std::string_view name, EUiLayer layer)
	{
		IUiApp* uiApp = IApp::Get()->GetUiApp();

		SelectElement(elem, name);
		uiApp->AddElementToLayer(layer, elem);
	}

	bool UiElementsTest::AddButton(IUiElement* elem, EUiLayer layer)
	{
		if (elem)
		{
			if (ImGui::Button(elem->GetName().c_str()))
			{
				SelectElementAndAddToLayer(elem, elem->GetName(), layer);
				return true;
			}
		}
		return false;
	}
}