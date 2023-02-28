#ifndef __I_UI_LOGIC_STATE_H__
#define __I_UI_LOGIC_STATE_H__

#include "nador/utils/Types.h"

namespace nador
{
	class IInputController;
	class IUiApp;

	class IUiLogicState
	{
	protected:
		virtual ~IUiLogicState() = default;

	public:
		virtual float_t GetDeltaTime() const = 0;
		virtual bool IsMouseOverHandled() const = 0;
		virtual void SetMouseOverHandled(bool handled) = 0;
		virtual const IInputController* GetInputCtrl() const noexcept = 0;
		virtual IUiApp* GetUiApp() const noexcept = 0;
	};
}

#endif // !__I_UI_LOGIC_STATE_H__
