#ifndef __NADOR_STREAM_LOGGER_H__
#define __NADOR_STREAM_LOGGER_H__

#include <memory>
#include <fstream>
#include <filesystem>

namespace nador
{
    class StreamLogger
    {
    public:
        using value_type = std::shared_ptr<std::ofstream>;

        explicit StreamLogger(value_type&& stream);

        void Log(const char* logMsg);

    private:
        value_type _streamPtr;
    };
}

#endif //!__NADOR_STREAM_LOGGER_H__