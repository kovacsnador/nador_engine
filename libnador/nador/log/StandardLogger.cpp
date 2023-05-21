#include <iostream>
#include <cstring>
#include <array>

#include "nador/log/StandardLogger.h"

static constexpr const char* stdGreenBegin = "\033[1;32m";
static constexpr const char* stdYellowBegin = "\033[1;33m";
static constexpr const char* stdRedBegin = "\033[1;31m";
static constexpr const char* stdMagentaBegin = "\033[1;35m";
static constexpr const char* stdColorEnd = "\033[0m";

template<size_t N>
static auto Concat(const char* color, const char* logMsg)
{
    std::array<char, N> result;
    
    std::strcpy(result.data(), color);
    strcat(result.data(), logMsg);
    strcat(result.data(), stdColorEnd);

    return result;
}

void nador::StandardLogger::Debug(const char* logMsg) const noexcept
{
    auto msgWithColor = Concat<2500>(stdGreenBegin, logMsg);
    std::cout << msgWithColor.data();
}

void nador::StandardLogger::Warning(const char* logMsg) const noexcept
{
    auto msgWithColor = Concat<2500>(stdYellowBegin, logMsg);
    std::cout << msgWithColor.data();
}

void nador::StandardLogger::Error(const char* logMsg) const noexcept
{
    auto msgWithColor = Concat<2500>(stdRedBegin, logMsg);
    std::cerr << msgWithColor.data();
}

void nador::StandardLogger::Fatal(const char* logMsg) const noexcept
{
    auto msgWithColor = Concat<2500>(stdMagentaBegin, logMsg);
    std::cerr << msgWithColor.data();
}

void nador::StandardLogger::EngineDebug(const char* logMsg) const noexcept
{
    Debug(logMsg);
}

void nador::StandardLogger::EngineWarning(const char* logMsg) const noexcept
{
    Warning(logMsg);
}

void nador::StandardLogger::EngineError(const char* logMsg) const noexcept
{
    Error(logMsg);
}

void nador::StandardLogger::EngineFatal(const char* logMsg) const noexcept
{
    Fatal(logMsg);
}