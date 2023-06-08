#ifndef __TINY_XML_HELPER_H__
#define __TINY_XML_HELPER_H__

#include "tinyXml2/tinyxml2.h"
#include "glm/glm.hpp"

namespace nador
{
	namespace xml
	{
		bool GetBoolean(const tinyxml2::XMLElement* elem, const char* name, bool def);
		std::string GetText(const tinyxml2::XMLElement* elem, const char* name, const char* def = 0);
		glm::uvec2 GetUVec2(const tinyxml2::XMLElement* elem, const char* name, const glm::uvec2& def = {});
		uint32_t GetUint32T(const tinyxml2::XMLElement* elem, const char* name, uint32_t def = 0);
	}
}

#endif // !__TINY_XML_HELPER_H__

