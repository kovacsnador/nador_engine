#ifndef __NADOR_STREAM_LOGGER_H__
#define __NADOR_STREAM_LOGGER_H__

#include <memory>
#include <fstream>
#include <filesystem>
#include <string_view>

namespace nador
{
    struct StreamLoggerException : std::runtime_error
    {
        using BaseT = std::runtime_error;
        using BaseT::BaseT;
    };

    template <typename StreamTy = std::ofstream>
    std::shared_ptr<StreamTy> GetStream(const std::filesystem::path& path)
    {
        auto parentPath = path.parent_path();
        if (std::filesystem::exists(parentPath) == false)
        {
            std::filesystem::create_directories(parentPath);
        }

        return std::make_shared<StreamTy>(path.generic_string());
    }

    template <size_t N>
    class StreamBuffer
    {
    public:
        bool CanFitInBuffer(size_t size) { return N - _currentPos > size; }

        bool Add(std::string_view msg)
        {
            if (CanFitInBuffer(msg.size()))
            {
                std::memcpy(&_buffer[_currentPos], &msg[0], msg.size());
                _currentPos += msg.size();
                return true;
            }
            return false;
        }

        const char* GetBuffer() { return &_buffer[0]; }

        void Clear()
        {
            _currentPos = 0;
            _buffer     = { 0 };
        }

    private:
        std::array<char, N> _buffer = { 0 };
        size_t              _currentPos { 0 };
    };

    template <typename StreamT, typename BufferT>
    class StreamLogger
    {
    public:
        explicit StreamLogger(std::shared_ptr<StreamT> stream, std::shared_ptr<BufferT> buffer = {})
        : _stream(std::move(stream))
        , _buffer(std::move(buffer))
        {
            if (_stream == nullptr)
            {
                throw StreamLoggerException("Stream is not created");
            }

            if (_stream->fail())
            {
                throw StreamLoggerException("Stream could not open");
            }
        }

        ~StreamLogger()
        {
            // Last stream to use the buffer
            if (_buffer.use_count() == 1)
            {
                Flush(_buffer->GetBuffer());
                _buffer->Clear();
            }
        }

        void Write(std::string_view logMsg)
        {
            bool bufferOk = false;

            if (_buffer)
            {
                bufferOk = _buffer->Add(logMsg);

                // Buffer is full
                if (bufferOk == false)
                {
                    // Trying to flush buffer
                    Flush(_buffer->GetBuffer());
                    _buffer->Clear();

                    // Trying to add again
                    bufferOk = _buffer->Add(logMsg);
                }
            }

            if(bufferOk == false)
            {
                // Flush directly on stream no buffer set
                Flush(logMsg);
            }
        }

        void FlushBuffer()
        { 
            if(_buffer)
            {
                Flush(_buffer->GetBuffer());
                _buffer->Clear();
            }
        }

        StreamLogger& operator<<(std::string_view logMsg)
        {
            Write(logMsg);
            return *this;
        }

    private:
        bool Flush(std::string_view msg)
        {
            if (_stream && _stream->is_open())
            {
                (*_stream) << msg;
                _stream->flush();
                return true;
            }
            return false;
        }

        std::shared_ptr<StreamT> _stream;
        std::shared_ptr<BufferT> _buffer;
    };
} // namespace nador

#endif //!__NADOR_STREAM_LOGGER_H__