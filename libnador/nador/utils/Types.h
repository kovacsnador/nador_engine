#ifndef __UTILS_TYPES_H__
#define __UTILS_TYPES_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <cstdint>
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include <cstring>

#include "nador/utils/CppMacros.h"

namespace nador
{
#define CREATE_PTR_TYPES(TYPE) \
using TYPE##Ptr = std::shared_ptr<TYPE>; \
using TYPE##UPtr = std::unique_ptr<TYPE>; \
using TYPE##WPtr = std::weak_ptr<TYPE>


using strings_t = std::vector<std::string>;
}



#endif // !__UTILS_TYPES_H__
