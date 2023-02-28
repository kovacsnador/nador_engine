#ifndef __TINY_XML_HELPER_H__
#define __TINY_XML_HELPER_H__

#include "tinyXml2/tinyxml2.h"
#include "glm/glm.hpp"

namespace nador
{
	namespace xml
	{
		bool GetBoolean(tinyxml2::XMLElement* elem, const char* name, bool def);
		std::string GetText(tinyxml2::XMLElement* elem, const char* name, const char* def = 0);
		glm::uvec2 GetUVec2(tinyxml2::XMLElement* elem, const char* name, const glm::uvec2 def = {});
	}
}

#endif // !__TINY_XML_HELPER_H__

