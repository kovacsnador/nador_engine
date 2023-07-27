#include "nador/ui/UiLogicState.h"
#include "nador/system/IInputController.h"
#include "nador/video/atlas/IAtlasController.h"
#include "nador/log/Log.h"

namespace nador
{
    UiLogicState::UiLogicState(float_t deltaTime, const IInputController* inputCtrl, const IAtlasController* atlasCtrl, IUiApp* uiApp)
    : _deltaTime(deltaTime)
    , _mouseOverHandled(false)
    , _inputCtrl(inputCtrl)
    , _atlasCtrl(atlasCtrl)
    , _uiApp(uiApp)
    {
        NADOR_ASSERT(_inputCtrl);
        NADOR_ASSERT(_atlasCtrl);
        NADOR_ASSERT(_uiApp);
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

    const IAtlasController* UiLogicState::GetAtlasCtrl() const noexcept
    {
        return _atlasCtrl;
    }
    
    IUiApp* UiLogicState::GetUiApp() const noexcept
    {
        return _uiApp;
    }

    glm::vec2 nador::UiLogicState::GetMousePosition() const noexcept
    {
        if (_inputCtrl)
        {
            return _inputCtrl->GetMousePosition();
        }
        return { -1, -1 };
    }
} // namespace nador