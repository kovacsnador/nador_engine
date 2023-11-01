#include "nador/common/GlobalListener.h"

namespace nador
{
	void GlobalListener::StartGlobalListening()
	{
		g_onTickEvent += _onTickListener;
		_onTickListener.SetCallback(std::bind(&GlobalListener::OnTick, this, arg::_1));

		g_onRenderEvent += _onRenderListener;
		_onRenderListener.SetCallback(std::bind(&GlobalListener::OnRender, this, arg::_1));

		g_onUiRenderEvent += _onUiRenderListener;
		_onUiRenderListener.SetCallback(std::bind(&GlobalListener::OnUiRender, this, arg::_1));

		g_onKeyPressedEvent += _onKeyPressedListener;
		_onKeyPressedListener.SetCallback(std::bind(&GlobalListener::OnKeyPressed, this, arg::_1));

		g_onKeyHoldedEvent += _onKeyHoldedListener;
		_onKeyHoldedListener.SetCallback(std::bind(&GlobalListener::OnKeyHolded, this, arg::_1));

		g_onKeyReleasedEvent += _onKeyReleasedListener;
		_onKeyReleasedListener.SetCallback(std::bind(&GlobalListener::OnKeyReleased, this, arg::_1));

		g_onMousePressedEvent += _onMousePressedListener;
		_onMousePressedListener.SetCallback(std::bind(&GlobalListener::OnMousePressed, this, arg::_1, arg::_2));

		g_onMouseReleasedEvent += _onMouseReleasedListener;
		_onMouseReleasedListener.SetCallback(std::bind(&GlobalListener::OnMouseReleased, this, arg::_1, arg::_2));

		g_onCharEvent += _onCharListener;
		_onCharListener.SetCallback(std::bind(&GlobalListener::OnCharEvent, this, arg::_1));

		_isListening = true;
	}

	void GlobalListener::StopGlobalListening()
	{
		_isListening = false;

		g_onTickEvent -= _onTickListener;
		g_onRenderEvent -= _onRenderListener;
		g_onUiRenderEvent -= _onUiRenderListener;

		g_onKeyPressedEvent -= _onKeyPressedListener;
		g_onKeyHoldedEvent -= _onKeyHoldedListener;
		g_onKeyReleasedEvent -= _onKeyReleasedListener;

		g_onMousePressedEvent -= _onMousePressedListener;
		g_onMouseReleasedEvent -= _onMouseReleasedListener;

		g_onCharEvent -= _onCharListener;
	}
}