#ifndef __TINY_XML_HELPER_H__
#define __TINY_XML_HELPER_H__

#include <charconv>
#include <string>
#include <optional>

#include "tinyXml2/tinyxml2.h"
#include "glm/glm.hpp"

#include "nador/log/ILog.h"
#include "nador/utils/Utils.h"

namespace nador
{
    namespace xml
    {
        template <typename T>
        T Get(const tinyxml2::XMLElement* elem, const T& def)
        {
            if (elem)
            {
                T result {};

                std::string str = nador::utils::trim(elem->GetText());
                auto [_, errc]  = std::from_chars(str.c_str(), str.c_str() + str.size(), result);
                if (errc == std::errc())
                {
                    // Everything is done
                    return result;
                }
                else if (errc == std::errc::invalid_argument)
                {
                    NADOR_ERROR("Invalid argument to convert from xml. Element: %s", elem->Name());
                }
                else if (errc == std::errc::result_out_of_range)
                {
                    NADOR_ERROR("Out of range to convert from xml. Element: %s", elem->Name());
                }
            }
            return def;
        }

        template <>
        inline std::string Get<std::string>(const tinyxml2::XMLElement* elem, const std::string& def)
        {
            if (elem)
            {
                return nador::utils::trim(elem->GetText());
            }
            return def;
        }

        template <>
        inline bool Get<bool>(const tinyxml2::XMLElement* elem, const bool& def)
        {
            if (elem)
            {
                bool ret;

                std::string text = Get<std::string>(elem, "false");
                if (tinyxml2::XMLUtil::ToBool(text.c_str(), &ret))
                {
                    return ret;
                }
            }
            return def;
        }

		template<typename T, typename Append>
		std::optional<T> GetArray(const tinyxml2::XMLElement* elem, Append callback)
		{
			if (elem)
            {
                T result = {};
                std::string text   = Get<std::string>(elem, "");
                if (text.empty() == false)
                {
                    std::istringstream ss(text);
                    std::string        s;

                    for (size_t i = 0; std::getline(ss, s, ' '); ++i)
                    {
						callback(result, s, i);
                    }
                    return result;
                }
            }
            return std::nullopt;
		}

        template <>
        inline glm::uvec2 Get<glm::uvec2>(const tinyxml2::XMLElement* elem, const glm::uvec2& def)
        {
            if (elem)
            {
				auto result = GetArray<glm::uvec2>(elem, [](glm::uvec2& a, const std::string& s, size_t i) {
					a[i] = stoi(s);
				});

				if(result)
				{
					return result.value();
				}
            }
            return def;
        }

        template <typename T>
        T GetByName(const tinyxml2::XMLElement* elem, std::string_view name, const T& def = {})
        {
            if (elem && name.data())
            {
                const tinyxml2::XMLElement* child = elem->FirstChildElement(name.data());
                return Get<T>(child, def);
            }
            return def;
        }

    } // namespace xml
} // namespace nador

#endif // !__TINY_XML_HELPER_H__
