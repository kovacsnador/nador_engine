#ifndef __UI_SCROLL_LIST_H__
#define __UI_SCROLL_LIST_H__

#include "nador/ui/UiImage.h"
#include "nador/ui/UiOverlay.h"
#include "nador/ui/UiAligner.h"

namespace nador
{
    class IRenderer;

    class UiScrollList : public IUiElement
    {
        enum class EState
        {
            IDLE,
            SCROLLING,
        };

        struct Spinner
        {
            float_t   lastDeltaTime { 0 };
            glm::vec2 lastOffset { 0, 0 };
            float_t   deceleration { 8.0f };    // 1.0f - ...

            glm::ivec2 Calculate(float_t currdDltaTime);
        };

    public:
        UiScrollList(const glm::ivec2& position, const glm::ivec2& size, UiAlignment alignment = UiAlignment(), IUiElement* parent = nullptr);

        void SetBackground(video::EImageName imageName);
        void SetSliderColor(const glm::vec4& color);

        UiImage*            GetBackground();
        UiRoundEdgeOverlay& GetSlider();
        UiAligner&          GetAligner();

        void AddElement(IUiElementPtr element);

        void OnRender(IRenderer* renderer, const quadVertices_t& vertices) override;
        void OnRenderEnd(IRenderer* renderer) override;
        void OnTick(IUiLogicState* uiLogicState, bool mouseOver) override;

        void OnMousePressed(EMouseButton mouseButton, const glm::vec2& position) override;
        void OnMouseReleased(EMouseButton mouseButton, const glm::vec2& position) override;

    private:
        void _UpdateSlider();

        EState _state { EState::IDLE };

        glm::ivec2 _lastMousePos { 0, 0 };

        UiImagePtr         _background;
        UiRoundEdgeOverlay _slider;
        UiAligner          _aligner;
        Spinner            _spinner;

        std::list<IUiElementPtr> _elements;
    };
    CREATE_PTR_TYPES(UiScrollList);
} // namespace nador

#endif // !__UI_SCROLL_LIST_H__
