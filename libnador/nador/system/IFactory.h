#ifndef __NADOR_I_FACTORY_H__
#define __NADOR_I_FACTORY_H__

#include "nador/utils/Types.h"

namespace nador
{
	class IWindow;
	class IVideo;
	class IFileController;
	class IInputController;
	class ISoundController;

	class IFactory
	{
	public:
		virtual ~IFactory() = default;

	public:
		virtual IWindow* GetWindow() const = 0;
		virtual const IWindow* GetWindowConst() const = 0;
		virtual const IVideo* GetVideo() const = 0;
		virtual IFileController* GetFileController() const = 0;
		virtual IInputController* GetInputController() const = 0;
		virtual ISoundController* GetSoundController() const = 0;
	};
	CREATE_PTR_TYPES(IFactory);
}

#endif // !__NADOR_I_FACTORY_H__
