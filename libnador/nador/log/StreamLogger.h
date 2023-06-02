#ifndef __NADOR_STREAM_LOGGER_H__
#define __NADOR_STREAM_LOGGER_H__

#include <memory>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <future>

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

    class StreamLogger
    {
    public:
        using value_type = std::shared_ptr<std::ofstream>;

        explicit StreamLogger(value_type&& stream)
        : _streamPtr(std::forward<value_type>(stream))
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

        void Write(std::string_view logMsg)
        {
            if (_streamPtr && _streamPtr->is_open())
            {
                (*_streamPtr) << logMsg;
                _streamPtr->flush();
            }
        }

    private:
        value_type _streamPtr;
    };
} // namespace nador

#endif //!__NADOR_STREAM_LOGGER_H__