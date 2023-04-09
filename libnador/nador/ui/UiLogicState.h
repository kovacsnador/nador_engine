#ifndef __UI_LOGIC_STATE_H__
#define __UI_LOGIC_STATE_H__

#include "nador/ui/IUiLogicState.h"

namespace nador
{
	class UiLogicState : public IUiLogicState
	{
	public:
		UiLogicState(float_t deltaTime, const IInputController* inputCtrl, IUiApp* uiApp);

		float_t GetDeltaTime() const override;
		bool IsMouseOverHandled() const override;
		void SetMouseOverHandled(bool handled) override;

		const IInputController* GetInputCtrl() const noexcept override;
		IUiApp* GetUiApp() const noexcept override;

		glm::vec2 GetMousePosition() const noexcept override;

	private:
		float_t _deltaTime;
		bool	_mouseOverHandled;

		const IInputController* _inputCtrl;
		IUiApp*					_uiApp;
	};
}

#endif // !__UI_LOGIC_STATE_H__
