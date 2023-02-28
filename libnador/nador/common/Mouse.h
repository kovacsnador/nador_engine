#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "nador/utils/Types.h"

namespace nador
{
	enum class EMouseButton : int32_t
	{
		UNKNOWN = -1,
		LEFT = 0,
		RIGHT = 1,
		MIDDLE = 2,
	};
}

#endif // !__MOUSE_H__
