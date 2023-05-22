#include "nador/log/StreamLogger.h"
#include "StreamLogger.h"

nador::StreamLogger::StreamLogger(value_type&& stream)
: _streamPtr(std::forward<value_type>(stream))
{
    if(_streamPtr == nullptr)
    {
        throw std::runtime_error("Stream is not created");
    }

    if(_streamPtr->fail())
    {
        throw std::runtime_error("Stream could not open");
    }
}

void nador::StreamLogger::Log(const char* logMsg)
{
    if(_streamPtr && _streamPtr->is_open())
    {
        (*_streamPtr) << logMsg;
    }
}
