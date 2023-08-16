#include "nador/ui/IUiApp.h"
#include "nador/ui/IUiElement.h"
#include "nador/video/renderer/IRenderer.h"
#include "nador/ui/IUiLogicState.h"

namespace nador
{
    IUiElement::IUiElement(const glm::ivec2& position, const glm::ivec2& size, UiAlignment alignment, IUiElement* parent, bool isShow)
    : _alignment(alignment)
    , _isShow(isShow)
    , _position(position)
    , _size(size)
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

    const glm::ivec2& IUiElement::GetSize() const noexcept
    {
        return _size;
    };

    void IUiElement::SetSize(const glm::ivec2& size) noexcept
    {
        if (size != _size)
        {
            _size = size;
        }
    };

    void IUiElement::SetSize(int32_t width, int32_t height) noexcept
    {
        SetSize({ width, height });
    };

    void IUiElement::SetWidth(int32_t width) noexcept
    {
        if (_size.x != width)
        {
            _size.x = width;
        }
    };

    void IUiElement::SetHeight(int32_t height) noexcept
    {
        if (_size.y != height)
        {
            _size.y = height;
        }
    };

    const glm::ivec2& IUiElement::GetPosition() const noexcept
    {
        return _position;
    };

    void IUiElement::SetPosition(const glm::ivec2& position) noexcept
    {
        if (position != _position)
        {
            _position = position;
        }
    };

    const UiAlignment& IUiElement::GetAligner() const noexcept
    {
        return _alignment;
    };

    void IUiElement::SetAlignment(const UiAlignment& aligner) noexcept
    {
        _alignment = aligner;
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
        elem->SetParent(this);

        utils::Remove(_childrens, elem);
        _childrens.push_back(elem);
    }

    void IUiElement::PushToBack(IUiElement* elem)
    {
        elem->SetParent(this);

        utils::Remove(_childrens, elem);
        _childrens.push_front(elem);
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

    const glm::vec3& IUiElement::GetScale() const noexcept
    {
        return _scale;
    }

    void IUiElement::SetScale(const glm::vec3& scale) noexcept
    {
        _scale = scale;

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

        OnRender(renderer, _vertices);

        if (drawDebugEdge)
        {
            _edgeDrawer.Render(renderer, _vertices, _scale);
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

    const quadVertices_t& IUiElement::GetVertices() const noexcept
    {
        return _vertices;
    }

    void IUiElement::SetOffset(const glm::ivec2& offset) noexcept
    {
        _offset = offset;
    }

    const glm::ivec2& IUiElement::GetOffset() const noexcept
    {
        return _offset;
    }

    void IUiElement::UpdateVertices(const quadVertices_t& parentVertices)
    {
        _vertices = _alignment.GenerateVertices(_position + _offset, _size, parentVertices);

        for (auto& it : _childrens)
        {
            it->UpdateVertices(_vertices);
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
        if (IsPointOverOnVertices(position, _vertices))
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
        if (IsPointOverOnVertices(position, _vertices))
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
        return IsPointOverOnVertices(point, _vertices);
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
} // namespace nador