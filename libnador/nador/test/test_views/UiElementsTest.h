#ifndef __UI_ELEMENTS_TEST_H__
#define __UI_ELEMENTS_TEST_H__

#include "nador/test/ITest.h"

#include "nador/ui/UiImage.h"
#include "nador/ui/UiTextLabel.h"
#include "nador/ui/UiButton.h"
#include "nador/ui/UiOverlay.h"
#include "nador/ui/UiScrollList.h"
#include "nador/ui/UiEditText.h"

namespace nador
{
    class IUiApp;
    class IFontController;
    class IInputController;
    class IRenderer;

    class UiElementsTest : public ITest
    {
    public:
        UiElementsTest(IUiApp* uiApp, const IFontController* fontCtrl, const IInputController* inputCtrl);

        void OnDebugRender(IRenderer* renderer) override;

    private:
        void SelectElement(IUiElement* elem, std::string_view name);
        void SelectElementAndAddToLayer(IUiElement* elem, std::string_view name, EUiLayer layer);
        bool AddButton(IUiElement* elem, EUiLayer layer);

        UiImagePtr            _uiImage { nullptr };
        UiTextLabelPtr        _uiTextLabel { nullptr };
        UiTextLabelPtr        _uiTextLabelMultiLine { nullptr };
        UiButtonPtr           _uiButtonWhite { nullptr };
        UiButtonPtr           _uiButtonBlack { nullptr };
        UiButtonPtr           _uiButtonWithText { nullptr };
        UiRoundEdgeOverlayPtr _uiRoundEdgeOverlay { nullptr };
        UiOverlayPtr          _uiOverlay { nullptr };
        UiScrollListPtr       _uiScrollList { nullptr };
        UiEditTextPtr         _uiEditText { nullptr };
        UiEditTextPtr         _uiEditTextSimple { nullptr };

        std::string _selectedName;
        IUiElement* _selectedUiElement { nullptr };

        bool      _selectedDragMode { false };
        glm::vec2 _lastMousePosition { 0, 0 };

        int32_t _selectedHorizontal { 0 };
        int32_t _selectedVertical { 0 };

        IUiApp*                 _uiApp;
        const IFontController*  _fontCtrl;
        const IInputController* _inputCtrl;
    };
} // namespace nador

#endif //! __UI_ELEMENTS_TEST_H__
