#include <cstring>

#include "nador/utils/Utils.h"

namespace nador
{
    namespace utils
    {
        glm::vec4 ScreenToWorldPosition(const glm::vec2& position, const glm::ivec2& screen, const glm::mat4& projMultiCamera)
        {
            float_t x = 2.0f * position.x / screen.x - 1.0f;
            float_t y = -2.0f * position.y / screen.y + 1.0f;
            glm::vec4 worldPosition(x, y, 0.0f, 1.0f);
            worldPosition = glm::inverse(projMultiCamera) * worldPosition;
            // flipping y coord
            worldPosition.y = screen.y - worldPosition.y;
            return worldPosition;
        }

        std::string trim(const char* s)
        {
            if (s == nullptr)
            {
                return "";
            }

            size_t len = std::strlen(s);
            const char* start = s;
            const char* end = s + len;

            while (start < end && std::isspace(*start))
            {
                start++;
            }

            do
            {
                end--;
            } while (start < end && std::isspace(*end));

            return std::string(start, end + 1);
        }

        int32_t CountMatchInRegex(const std::string& s, const std::regex& regex)
        {
            auto words_begin = std::sregex_iterator(
                s.begin(), s.end(), regex);
            auto words_end = std::sregex_iterator();

            return std::distance(words_begin, words_end);
        }

        void PopBackUtf8(std::string& utf8)
        {
            if (utf8.empty())
            {
                return;
            }

            auto cp = utf8.data() + utf8.size();

            while (--cp >= utf8.data() && ((*cp & 0b10000000) && !(*cp & 0b01000000)))
            {
            }

            if (cp >= utf8.data())
            {
                utf8.resize(cp - utf8.data());
            }
        }
    }
}