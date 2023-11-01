#ifndef __GLOBAL_LISTENER_H__
#define __GLOBAL_LISTENER_H__

#include "nador/common/GlobalEvents.h"

namespace nador
{
	class IRenderer;

	class GlobalListener
	{
	protected:
		virtual ~GlobalListener() = default;

		virtual void StartGlobalListening() final;
		virtual void StopGlobalListening() final;

		bool isListening() const noexcept { return _isListening; }

		virtual void OnTick(float_t) {};
		virtual void OnRender(IRenderer*) {};
		virtual void OnUiRender(IRenderer*) {};

		virtual void OnKeyPressed(EKeyCode) {};
		virtual void OnKeyHolded(EKeyCode) {};
		virtual void OnKeyReleased(EKeyCode) {};

		virtual void OnCharEvent(const std::string& /*text*/) {};

		virtual void OnMousePressed(EMouseButton, const glm::vec2&) {};
		virtual void OnMouseReleased(EMouseButton, const glm::vec2&) {};

	private:
		bool _isListening{ false };

		onRender_listener_t _onRenderListener;
		onUiRender_listener_t _onUiRenderListener;
		onTick_listener_t	_onTickListener;

		onKey_listener_t _onKeyPressedListener;
		onKey_listener_t _onKeyHoldedListener;
		onKey_listener_t _onKeyReleasedListener;

		onChar_listener_t _onCharListener;

		onMouse_listener_t _onMousePressedListener;
		onMouse_listener_t _onMouseReleasedListener;
	};
}

#endif // !__GLOBAL_LISTENER_H__