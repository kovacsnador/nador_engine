#include "nador/log/StreamLogger.h"
#include "StreamLogger.h"

nador::StreamLogger::StreamLogger(value_type&& stream)
: _streamPtr(std::forward<value_type>(stream))
{
}

void nador::StreamLogger::Log(const char* logMsg)
{
    if(_streamPtr && _streamPtr->is_open())
    {
        (*_streamPtr) << logMsg;
    }
}
