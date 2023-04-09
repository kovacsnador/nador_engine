#include "nador/ui/IUiApp.h"

#include "nador/ui/IUiElement.h"

namespace nador
{
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
			if(_uiApp)
			{
				_uiApp->RemoveElement(this);
			}
		}

		if (_uiApp && _uiApp->IsInFocus(this))
		{
			_uiApp->SetInFocus(nullptr);
		}
	}

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

		//UpdateVertices(GetParentVertices());
	};

	void IUiElement::BringToFront()
	{
		if (_parent)
		{
			_parent->AddChild(this);
		}
		else if(_uiApp)
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
		else if(_uiApp)
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

	void IUiElement::SuspendInputEvents(bool suspend)
	{ 
		_inputEventSuspended = suspend;
	}

	void IUiElement::SuspendInputEventsForChildrens(bool suspend)
	{
		for (auto& it : _childrens)
		{
			it->SuspendInputEvents(suspend);
		}
	}

	void IUiElement::SetScale(const glm::vec3& scale)
	{
		_scale = scale;

		for (auto& it : _childrens)
		{
			it->SetScale(scale);
		}
	}

	void IUiElement::OnTickImpl(IUiLogicState* uiLogicState)
	{
		OnTick(uiLogicState);

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

	void IUiElement::SetOffset(const glm::ivec2& offset)
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

	bool IUiElement::OnMousePressedWithPositionCheck(EMouseButton mouseButton, const glm::vec2& position)
	{
		if (IsPointOverOnVertices(position, _vertices))
		{
			if(_uiApp)
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
}