#ifndef __NADOR_STREAM_LOGGER_H__
#define __NADOR_STREAM_LOGGER_H__

#include <memory>
#include <fstream>
#include <filesystem>
#include <chrono>

namespace nador
{
    template <typename StreamTy>
    std::unique_ptr<StreamTy> GetStream(const std::filesystem::path& path)
    {
        auto parentPath = path.parent_path();
        if (std::filesystem::exists(parentPath) == false)
        {
            std::filesystem::create_directories(parentPath);
        }

        return std::make_unique<StreamTy>(path.generic_string());
    }

    template <typename ClockTy>
    class StreamLogger
    {
    public:
        using value_type = std::shared_ptr<std::ofstream>;

        template <typename DurationTy>
        explicit StreamLogger(value_type&& stream, DurationTy flushFrequency)
        : _streamPtr(std::forward<value_type>(stream))
        , _flushFrequency(flushFrequency)
        , _lastFlush(ClockTy::now())
        {
            if (_streamPtr == nullptr)
            {
                throw std::runtime_error("Stream is not created");
            }

            if (_streamPtr->fail())
            {
                throw std::runtime_error("Stream could not open");
            }
        }

        void Log(std::string_view logMsg)
        {
            if (_streamPtr && _streamPtr->is_open())
            {
                (*_streamPtr) << logMsg;

                auto now = ClockTy::now();
                if (now - _lastFlush >= _flushFrequency)
                {
                    Flush();
                }
            }
        }

        void Flush()
        {
            if (_streamPtr && _streamPtr->is_open())
            {
                _streamPtr->flush();
                _lastFlush = ClockTy::now();
            }
        }

    private:
        value_type                                _streamPtr;
        std::chrono::duration<double, std::milli> _flushFrequency;
        std::chrono::time_point<ClockTy>          _lastFlush;
    };
} // namespace nador

#endif //!__NADOR_STREAM_LOGGER_H__