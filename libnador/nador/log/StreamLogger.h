#ifndef __NADOR_STREAM_LOGGER_H__
#define __NADOR_STREAM_LOGGER_H__

#include <memory>
#include <fstream>
#include <filesystem>
#include <string_view>

namespace nador
{
    template <typename StreamTy = std::ofstream>
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

        explicit StreamLogger(value_type stream)
        : _stream(std::move(stream))
        {
            if (_stream == nullptr)
            {
                throw std::runtime_error("Stream is not created");
            }

            if (_stream->fail())
            {
                throw std::runtime_error("Stream could not open");
            }
        }

        void Write(std::string_view logMsg)
        {
            if (_stream && _stream->is_open())
            {
                (*_stream) << logMsg;
                _stream->flush();
            }
        }

        StreamLogger& operator<<(std::string_view logMsg)
        {
            Write(logMsg);
            return *this;
        }

    private:
        value_type _stream;
    };
} // namespace nador

#endif //!__NADOR_STREAM_LOGGER_H__