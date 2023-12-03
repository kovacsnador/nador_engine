#include "nador/ui/IUiApp.h"
#include "nador/ui/IUiElement.h"
#include "nador/video/renderer/IRenderer.h"
#include "nador/ui/IUiLogicState.h"

namespace nador
{
    UiSquare::UiSquare(const glm::ivec2& pos, const glm::ivec2& size, UiAlignment alignment)
    : _position(pos)
    , _size(size)
    , _alignment(alignment)
    {
    }

    IUiElement::IUiElement(const glm::ivec2& position, const glm::ivec2& size, UiAlignment alignment, IUiElement* parent, bool isShow)
    : UiSquare(position, size, alignment)
    , _isShow(isShow)
    {
        SetParent(parent);
    }

    IUiElement::~IUiElement()
    {
        for (auto& it : _childrens)
        {
            it->SetParent(nullptr);
        }

        if (_parent)
        {
            _parent->RemoveChild(this);
        }
        else
        {
            if (_uiApp)
            {
                _uiApp->RemoveElement(this);
            }
        }

        if (_uiApp && _uiApp->IsInFocus(this))
        {
            _uiApp->SetInFocus(nullptr);
        }
    }

    void IUiElement::Show() noexcept
    {
        _isShow = true;
    };

    void IUiElement::Show(bool show) noexcept
    {
        _isShow = show;
    }

    void IUiElement::Hide() noexcept
    {
        _isShow = false;
    };

    bool IUiElement::IsShow() const noexcept
    {
        return _isShow;
    };

    bool IUiElement::IsHide() const noexcept
    {
        return !_isShow;
    };

    IUiElement* IUiElement::GetParent() const noexcept
    {
        return _parent;
    };

    void IUiElement::SetParent(IUiElement* elem)
    {
        if (_parent != elem)
        {
            _parent = elem;
            if (elem)
            {
                elem->AddChild(this);
            }
        }
    };

    void IUiElement::BringToFront()
    {
        if (_parent)
        {
            _parent->AddChild(this);
        }
        else if (_uiApp)
        {
            _uiApp->BringToFront(this);
        }
    }

    void IUiElement::PushToBack()
    {
        if (_parent)
        {
            _parent->PushToBack(this);
        }
        else if (_uiApp)
        {
            _uiApp->PushToback(this);
        }
    }

    void IUiElement::AddChild(IUiElement* elem)
    {
        if (elem)
        {
            elem->SetParent(this);

            utils::Remove(_childrens, elem);
            _childrens.push_back(elem);
        }
    }

    void IUiElement::PushToBack(IUiElement* elem)
    {
        if (elem)
        {
            elem->SetParent(this);

            utils::Remove(_childrens, elem);
            _childrens.push_front(elem);
        }
    }

    void IUiElement::RemoveChild(IUiElement* elem)
    {
        if (utils::Find(_childrens, elem))
        {
            elem->SetParent(nullptr);
            utils::Remove(_childrens, elem);
        }
        else
        {
            ENGINE_WARNING("Element not as child attached.");
        }
    }

    void IUiElement::SuspendInputEvents(bool suspend) noexcept
    {
        _inputEventSuspended = suspend;
    }

    void IUiElement::SuspendInputEventsForChildrens(bool suspend) noexcept
    {
        for (auto& it : _childrens)
        {
            it->SuspendInputEvents(suspend);
        }
    }

    void IUiElement::HandleInputEventBeforeChildren(bool beforeChildren) noexcept
    {
        _handleInputEventBeforeChildren = beforeChildren;
    }

    void IUiElement::SetDefaultMouseHandling(bool handled) noexcept
    {
        _defaultMouseHandling = handled;
    }

    void IUiElement::SetScale(const glm::vec3& scale) noexcept
    {
        SetScaleImpl(scale);

        for (auto& it : _childrens)
        {
            it->SetScale(scale);
        }
    }

    void IUiElement::OnTickImpl(IUiLogicState* uiLogicState)
    {
        // Check and handle mouse over event
        bool mouseOver { false };

        if (IsOver(uiLogicState->GetMousePosition()) && uiLogicState->IsMouseOverHandled() == false && _inputEventSuspended == false)
        {
            uiLogicState->SetMouseOverHandled(true);
            mouseOver = true;
        }

        OnTick(uiLogicState, mouseOver);

        // tick in reverse order
        for (auto it = _childrens.rbegin(); it != _childrens.rend(); ++it)
        {
            NADOR_ASSERT(*it);

            (*it)->OnTickImpl(uiLogicState);
        }
    }

    void IUiElement::OnRenderImpl(IRenderer* renderer, bool drawDebugEdge)
    {
        if (IsHide())
        {
            return;
        }

        const auto& vertices = GetVertices();

        OnRender(renderer, vertices);

        if (drawDebugEdge)
        {
            _edgeDrawer.Render(renderer, vertices, GetScale());
        }

        for (auto& it : _childrens)
        {
            it->OnRenderImpl(renderer, drawDebugEdge);
        }

        OnRenderEnd(renderer);
    }

    bool IUiElement::OnMousePressedImpl(EMouseButton mouseButton, const glm::vec2& position)
    {
        return OnKeyAndMouseEventImpl(&IUiElement::OnMousePressedImpl, &IUiElement::OnMousePressedWithPositionCheck, mouseButton, position);
    }

    bool IUiElement::OnMouseReleasedImpl(EMouseButton mouseButton, const glm::vec2& position)
    {
        return OnKeyAndMouseEventImpl(&IUiElement::OnMouseReleasedImpl, &IUiElement::OnMouseReleasedWithPositionCheck, mouseButton, position);
    }

    bool IUiElement::OnKeyPressedImpl(EKeyCode keyCode)
    {
        return OnKeyAndMouseEventImpl(&IUiElement::OnKeyPressedImpl, &IUiElement::OnKeyPressedLogic, keyCode);
    }

    bool IUiElement::OnKeyHoldedImpl(EKeyCode keyCode)
    {
        return OnKeyAndMouseEventImpl(&IUiElement::OnKeyHoldedImpl, &IUiElement::OnKeyHoldedLogic, keyCode);
    }

    bool IUiElement::OnKeyReleasedImpl(EKeyCode keyCode)
    {
        return OnKeyAndMouseEventImpl(&IUiElement::OnKeyReleasedImpl, &IUiElement::OnKeyReleasedLogic, keyCode);
    }

    bool IUiElement::OnCharImpl(const std::string& text)
    {
        return OnKeyAndMouseEventImpl(&IUiElement::OnCharImpl, &IUiElement::OnCharPressed, text);
    }

    void IUiElement::UpdateVertices(const quadVertices_t& parentVertices)
    {
        const auto& vertices = UpdateVerticesImpl(parentVertices);

        for (auto& it : _childrens)
        {
            it->UpdateVertices(vertices);
        }
    }

    void IUiElement::SetOnMousePressedCallback(const OnMousePressedCallback_t& cb)
    {
        _onMousePressedCb = cb;
    }
    void IUiElement::SetOnMouseReleasedCallback(const OnMouseReleasedCallback_t& cb)
    {
        _onMouseReleasedCb = cb;
    }

    void IUiElement::SetOnKeyPressedCallback(const OnKeyPressedCallback_t& cb)
    {
        _onKeyPressedCb = cb;
    }
    void IUiElement::SetOnKeyHoldedCallback(const OnKeyHoldedCallback_t& cb)
    {
        _onKeyHoldedCb = cb;
    }
    void IUiElement::SetOnKeyReleasedCallback(const OnKeyReleasedCallback_t& cb)
    {
        _onKeyReleasedCb = cb;
    }

    bool IUiElement::OnMousePressedWithPositionCheck(EMouseButton mouseButton, const glm::vec2& position)
    {
        if (IsPointOverOnVertices(position, GetVertices()))
        {
            if (_uiApp)
            {
                _uiApp->SetInFocus(this);
            }

            OnMousePressed(mouseButton, position);

            // Call user defined callback
            return InvokeCallback(_onMousePressedCb, _defaultMouseHandling, mouseButton, position, this);
        }

        return false;
    }

    bool IUiElement::OnMouseReleasedWithPositionCheck(EMouseButton mouseButton, const glm::vec2& position)
    {
        if (IsPointOverOnVertices(position, GetVertices()))
        {
            OnMouseReleased(mouseButton, position);

            // Call user defined callback
            return InvokeCallback(_onMouseReleasedCb, _defaultMouseHandling, mouseButton, position, this);
        }

        return false;
    }

    bool IUiElement::OnKeyPressedLogic(EKeyCode keyCode)
    {
        bool result = false;

        result |= OnKeyPressed(keyCode);
        result |= InvokeCallback(_onKeyPressedCb, false, keyCode);

        return result;
    };

    bool IUiElement::OnKeyHoldedLogic(EKeyCode keyCode)
    {
        bool result = false;

        result |= OnKeyHolded(keyCode);
        result |= InvokeCallback(_onKeyHoldedCb, false, keyCode);

        return result;
    };

    bool IUiElement::OnKeyReleasedLogic(EKeyCode keyCode)
    {
        bool result = false;

        result |= OnKeyReleased(keyCode);
        result |= InvokeCallback(_onKeyReleasedCb, false, keyCode);

        return result;
    };

    bool IUiElement::IsOver(const glm::vec2& point) const noexcept
    {
        return IsPointOverOnVertices(point, GetVertices());
    }

    const std::string& IUiElement::GetName() const noexcept
    {
        return _name;
    }

    void IUiElement::SetName(const std::string& name)
    {
        _name = name;
    }

    void IUiElement::SetUiAppHandler(IUiApp* uiApp) noexcept
    {
        _uiApp = uiApp;

        for (auto& it : _childrens)
        {
            it->SetUiAppHandler(_uiApp);
        }
    }

    IUiApp* IUiElement::GetUiApp() const noexcept
    {
        return _uiApp;
    }
} // namespace nador