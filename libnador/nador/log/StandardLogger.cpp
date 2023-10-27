#include <iostream>
#include <cstring>
#include <array>

#include "nador/log/StandardLogger.h"
#include "StandardLogger.h"

static constexpr size_t s_standardLoggerBuffer = 2500;

static constexpr const char* stdGreenBegin   = "\033[1;32m";
static constexpr const char* stdYellowBegin  = "\033[1;33m";
static constexpr const char* stdRedBegin     = "\033[1;31m";
static constexpr const char* stdMagentaBegin = "\033[1;35m";
static constexpr const char* stdColorEnd     = "\033[0m";

template <size_t N>
static auto Concat(const char* color, std::string_view logMsg)
{
    std::array<char, N> result;

    std::strcpy(result.data(), color);
    strcat(result.data(), logMsg.data());
    strcat(result.data(), stdColorEnd);

    return result;
}

void nador::StandardLogger::Debug(std::string_view logMsg) const noexcept
{
    auto msgWithColor = Concat<s_standardLoggerBuffer>(stdGreenBegin, logMsg);
    std::cout << msgWithColor.data();
}

void nador::StandardLogger::Warning(std::string_view logMsg) const noexcept
{
    auto msgWithColor = Concat<s_standardLoggerBuffer>(stdYellowBegin, logMsg);
    std::cout << msgWithColor.data();
}

void nador::StandardLogger::Error(std::string_view logMsg) const noexcept
{
    auto msgWithColor = Concat<s_standardLoggerBuffer>(stdRedBegin, logMsg);
    std::cerr << msgWithColor.data();
}

void nador::StandardLogger::Fatal(std::string_view logMsg) const noexcept
{
    auto msgWithColor = Concat<s_standardLoggerBuffer>(stdMagentaBegin, logMsg);
    std::cerr << msgWithColor.data();
}

void nador::StandardLogger::EngineDebug(std::string_view logMsg) const noexcept
{
    Debug(logMsg);
}

void nador::StandardLogger::EngineWarning(std::string_view logMsg) const noexcept
{
    Warning(logMsg);
}

void nador::StandardLogger::EngineError(std::string_view logMsg) const noexcept
{
    Error(logMsg);
}

void nador::StandardLogger::EngineFatal(std::string_view logMsg) const noexcept
{
    Fatal(logMsg);
}

nador::StandardLogger& nador::StandardLogger::Write(ELogType type, std::string_view logMsg)
{
    switch (type)
    {
    case nador::ELogType::DEBUG:
        Debug(logMsg);
        break;
    case nador::ELogType::WARNING:
        Warning(logMsg);
        break;
    case nador::ELogType::ERROR:
        Error(logMsg);
        break;
    case nador::ELogType::FATAL:
        Fatal(logMsg);
        break;

    case nador::ELogType::ENGINE_DEBUG:
        EngineDebug(logMsg);
        break;
    case nador::ELogType::ENGINE_WARNING:
        EngineWarning(logMsg);
        break;
    case nador::ELogType::ENGINE_ERROR:
        EngineError(logMsg);
        break;
    case nador::ELogType::ENGINE_FATAL:
        EngineFatal(logMsg);
        break;

    default:
        break;
    }

    return *this;
}