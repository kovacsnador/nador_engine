#include <sstream>

#include "nador/utils/TinyXmlHelper.h"
#include "nador/utils/Utils.h"

namespace nador
{
	namespace xml
	{
		bool GetBoolean(const tinyxml2::XMLElement* elem, const char* name, bool def)
		{
			bool ret;
			std::string text = GetText(elem, name, "false");
			if (tinyxml2::XMLUtil::ToBool(text.c_str(), &ret))
			{
				return ret;
			}
			return def;
		}

		std::string GetText(const tinyxml2::XMLElement* elem, const char* name, const char* def)
		{
			NADOR_ASSERT(elem);
			NADOR_ASSERT(name);

			const tinyxml2::XMLElement* child = elem->FirstChildElement(name);
			if (child)
			{
				return nador::utils::trim(child->GetText());
			}

			return def;
		}

		glm::uvec2 GetUVec2(const tinyxml2::XMLElement* elem, const char* name, const glm::uvec2& def)
		{
			NADOR_ASSERT(elem);
			NADOR_ASSERT(name);

			const tinyxml2::XMLElement* child = elem->FirstChildElement(name);
			if (child)
			{
				glm::uvec2 result = {};
				std::string text = nador::utils::trim(child->GetText());
	 			if (text.empty() == false)
				{
					std::istringstream ss(text);
					std::string s;

					for (size_t i = 0; std::getline(ss, s, ' '); ++i)
					{
						result[i] = stoi(s);
					}
					return result;
				}
			}

			return def;
		}
	}
}