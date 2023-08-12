#include "nador/IApp.h"
#include "nador/log/ILog.h"

namespace nador
{
    IApp* IApp::s_instance = nullptr;

    IApp::IApp()
    {
        if (s_instance)
        {
            ENGINE_FATAL("Application is already created and initialized!");
        }

        s_instance = this;
        ENGINE_DEBUG("App instance created.");
    }

    IApp::~IApp()
    {
        s_instance = nullptr;
        ENGINE_DEBUG("App instance destroyed.");
    }

    IApp* IApp::Get()
    {
        if (s_instance == nullptr)
        {
            ENGINE_FATAL("Application is not created yet!");
        }

        return s_instance;
    }
}
    