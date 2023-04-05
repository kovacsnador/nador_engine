#include "nador/ui/UiApp.h"
#include "nador/App.h"
#include "nador/ui/UiLogicState.h"

namespace nador
{
	const IUiElement* UiApp::s_pFocusedElement{ nullptr };

	UiApp::UiApp()
	{
		StartGlobalListening();
	}

	const glm::ivec2& UiApp::GetScreenSize() const
	{
		return IApp::Get()->GetVideo()->GetScreenSize();
	}

	float_t UiApp::GetScreenScale() const
	{
		// TODO: make it dynamic
		return 1;
	}

	float_t UiApp::GetAspectRatio() const
	{
		glm::vec2 screenSize = IApp::Get()->GetVideo()->GetScreenSize();
		return screenSize.x / screenSize.y;
	}

	const std::string& UiApp::GetUiRootFolder() const
	{
		return IApp::Get()->GetAppConfig().uiPath;
	}

	quadVertices_t UiApp::GetScreenVertices() const
	{
		const glm::vec2& scrennSize = GetScreenSize();

		return {
			0.0f,			0.0f,			1.0f, 1.0f,
			scrennSize.x,	0.0f,			1.0f, 1.0f,
			scrennSize.x,	scrennSize.y,	1.0f, 1.0f,
			0.0f,			scrennSize.y,	1.0f, 1.0f,
		};
	}

	void UiApp::AddElementToLayer(EUiLayer eLayer, IUiElement* elem)
	{
		auto layer = _layers.find(eLayer);
		if (layer == _layers.end())
		{
			auto pairResult = _layers.insert_or_assign(eLayer, ui_element_list_t());
			layer = pairResult.first;
		}

		ui_element_list_t& list = layer->second;

		utils::Remove(list, elem);

		list.push_back(elem);
	}

	void UiApp::RemoveElementFromLayer(EUiLayer eLayer, const IUiElement* elem)
	{
		auto layer = _layers.find(eLayer);
		if (layer != _layers.end())
		{
			utils::Remove(layer->second, elem);
		}
	}

	void UiApp::RemoveElement(const IUiElement* elem)
	{
		for (auto& layer : _layers)
		{
			utils::Remove(layer.second, elem);
		}
	}

	void UiApp::ClearLayer(EUiLayer eLayer)
	{
		_layers.insert_or_assign(eLayer, ui_element_list_t());
	}

	void UiApp::DebugDrawEdge(bool draw)
	{
		_debugDrawEdge = draw;
	}

	bool UiApp::IsDebugDrawEdge() const noexcept
	{
		return _debugDrawEdge;
	}

	void UiApp::BringToFront(IUiElement* elem)
	{
		_PushElementInLayers(elem, [](ui_element_list_t& list, IUiElement* elem) {
			list.push_back(elem);
		});
	}

	void UiApp::PushToback(IUiElement* elem)
	{
		_PushElementInLayers(elem, [](ui_element_list_t& list, IUiElement* elem) {
			list.push_front(elem);
		});
	}

	bool UiApp::IsInFocus(const IUiElement* elem) const noexcept
	{
		return (s_pFocusedElement == elem);
	}

	void UiApp::SetInFocus(const IUiElement* elem) const noexcept
	{
		s_pFocusedElement = elem;
	}

	void UiApp::OnTick(float_t deltaTime)
	{
		const IInputController* inputCtrl = IApp::Get()->GetInputController();
		UiLogicState uiLogicState(deltaTime, inputCtrl, this);

		// reverse iteration 
		for (auto layer = _layers.rbegin(); layer != _layers.rend(); layer++)
		{
			for (auto elem = layer->second.rbegin(); elem != layer->second.rend(); elem++)
			{
				(*elem)->OnTickImpl(&uiLogicState);
			}
		}
	}

	void UiApp::OnRender(IRenderer* renderer)
	{
		for (auto& layer : _layers)
		{
			for (auto& elem : layer.second)
			{
				if (elem)
				{
					elem->OnRenderImpl(renderer, _debugDrawEdge);
				}
			}
		}
	}

	void UiApp::OnKeyPressed(EKeyCode keyCode)
	{
		_OnKeyAndMouseEventImpl(&IUiElement::OnKeyPressedImpl, keyCode);
	}

	void UiApp::OnKeyHolded(EKeyCode keyCode)
	{
		_OnKeyAndMouseEventImpl(&IUiElement::OnKeyHoldedImpl, keyCode);
	}

	void UiApp::OnKeyReleased(EKeyCode keyCode)
	{
		_OnKeyAndMouseEventImpl(&IUiElement::OnKeyReleasedImpl, keyCode);
	}

	void UiApp::OnCharEvent(const std::string& text)
	{
		_OnKeyAndMouseEventImpl(&IUiElement::OnCharImpl, text);
	}

	void UiApp::OnMousePressed(EMouseButton eMouseButton, const glm::vec2& position)
	{
		_OnKeyAndMouseEventImpl(&IUiElement::OnMousePressedImpl, eMouseButton, position);
	}

	void UiApp::OnMouseReleased(EMouseButton eMouseButton, const glm::vec2& position)
	{
		_OnKeyAndMouseEventImpl(&IUiElement::OnMouseReleasedImpl, eMouseButton, position);
	}

	void UiApp::_PushElementInLayers(IUiElement* elem, std::function<void(ui_element_list_t&, IUiElement*)> callback)
	{
		for (auto& layer : _layers)
		{
			auto& list = layer.second;

			if (std::find(list.begin(), list.end(), elem) != list.end())
			{
				utils::Remove(list, elem);
				callback(list, elem);
			}
		}
	}
}
