#ifndef __UI_PARSER_H__
#define __UI_PARSER_H__

#include "nador/ui/IUiElement.h"

namespace nador
{
	class UiParser
	{
		static IUiElementPtr Parse(const std::string& xmlPath);
	};
}

#endif // !__UI_PARSER_H__
