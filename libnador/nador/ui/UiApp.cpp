#include "nador/ui/UiApp.h"
#include "nador/ui/UiLogicState.h"

namespace nador
{
	const IUiElement* UiApp::s_pFocusedElement{ nullptr };

	UiApp::UiApp(const IVideoPtr video, const IInputControllerPtr inputCtrl, const IAtlasControllerPtr atlasCtrl)
	: _video(std::move(video))
	, _inputCtrl(std::move(inputCtrl))
	, _atlasCtrl(std::move(atlasCtrl))
	{
		NADOR_ASSERT(_video);
		NADOR_ASSERT(_inputCtrl);
		NADOR_ASSERT(_atlasCtrl);

		StartGlobalListening();
	}

	UiApp::~UiApp()
	{
		// remove ui app handler from all elements
		for (auto& layer : _layers)
		{
			for (auto& elem : layer.second)
			{
				if (elem)
				{
					elem->SetUiAppHandler(nullptr);
				}
			}
		}
	}

	const glm::ivec2& UiApp::GetScreenSize() const
	{
		return _video->GetScreenSize();
	}

	float_t UiApp::GetScreenScale() const
	{
		// TODO: make it dynamic
		return 1;
	}

	float_t UiApp::GetAspectRatio() const
	{
		glm::vec2 screenSize = _video->GetScreenSize();
		return screenSize.x / screenSize.y;
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
		elem->SetUiAppHandler(this);
	}

	void UiApp::RemoveElementFromLayer(EUiLayer eLayer, IUiElement* elem)
	{
		auto layer = _layers.find(eLayer);
		if (layer != _layers.end())
		{
			utils::Remove(layer->second, elem);
			elem->SetUiAppHandler(nullptr);
		}
	}

	void UiApp::RemoveElement(IUiElement* elem)
	{
		for (auto& layer : _layers)
		{
			utils::Remove(layer.second, elem);
			elem->SetUiAppHandler(nullptr);
		}
	}

	void UiApp::ClearLayer(EUiLayer eLayer)
	{
		auto layer = _layers.find(eLayer);
		if (layer != _layers.end())
		{
			std::for_each(layer->second.begin(), layer->second.end(), [](IUiElement* elem) { elem->SetUiAppHandler(nullptr); });
		}

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
		UiLogicState uiLogicState(deltaTime, _inputCtrl.get(), _atlasCtrl.get(), this);
		auto vertices = GetScreenVertices();

		// reverse iteration 
		for (auto layer = _layers.rbegin(); layer != _layers.rend(); layer++)
		{
			for (auto elem = layer->second.rbegin(); elem != layer->second.rend(); elem++)
			{
				(*elem)->UpdateVertices(vertices);
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
}
