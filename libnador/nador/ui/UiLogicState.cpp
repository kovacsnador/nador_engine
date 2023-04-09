#include "nador/ui/UiLogicState.h"
#include "nador/system/IInputController.h"
#include "nador/log/Log.h"

namespace nador
{
    UiLogicState::UiLogicState(float_t deltaTime, const IInputController* inputCtrl, IUiApp* uiApp)
    : _deltaTime(deltaTime)
    , _mouseOverHandled(false)
    , _inputCtrl(inputCtrl)
    , _uiApp(uiApp)
    {
		NADOR_ASSERT(_inputCtrl);
    }

    float_t UiLogicState::GetDeltaTime() const
    {
        return _deltaTime;
    }

    bool UiLogicState::IsMouseOverHandled() const
    {
        return _mouseOverHandled;
    }

    void UiLogicState::SetMouseOverHandled(bool handled)
    {
        _mouseOverHandled = handled;
    }

    const IInputController* UiLogicState::GetInputCtrl() const noexcept
    {
        return _inputCtrl;
    }

    IUiApp* UiLogicState::GetUiApp() const noexcept
    {
        return _uiApp;
    }

    glm::vec2 nador::UiLogicState::GetMousePosition() const noexcept
    {
        if(_inputCtrl)
		{
			return _inputCtrl->GetMousePosition();
		}
		return {0, 0};
    }
} // namespace nador