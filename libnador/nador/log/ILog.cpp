

#include "nador/log/ILog.h"

namespace nador
{
    static std::unique_ptr<ILog> g_logInterface {nullptr};

    ILog* GetLoggingInterface()
    {
        return g_logInterface.get();
    }

    void SetLoggingInterface(std::unique_ptr<ILog> log)
    {
        g_logInterface = std::move(log);
    }
}
