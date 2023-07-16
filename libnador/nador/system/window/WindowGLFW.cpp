#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "nador/log/Log.h"
#include "nador/system/window/WindowGLFW.h"
#include "nador/utils/ImGuiHelper.h"
#include "nador/system/input/input_events/InputEvents.h"

namespace nador
{

    static void FramebufferSizeCallback(GLFWwindow* /*window*/, int32_t width, int32_t height)
    {
        glViewport(0, 0, width, height);
    }

    static void WindowCloseCallback(GLFWwindow* /*window*/)
    {
        // fire window close event
        //g_onWindowCloseEvent();
        HandleInputEvent(OnWindowClosedEvent{});
    }

    WindowGLFW::~WindowGLFW()
    {
        if(_imGuiAdapter)
        {
            _imGuiAdapter->Imgui_Shutdown();
        }

        glfwTerminate();

        ENGINE_DEBUG("WindowGLFW destroyed.");
    }

    void WindowGLFW::CreateWindow(int32_t width, int32_t height, const char* title, bool vSync)
    {
        if (_window)
        {
            ENGINE_WARNING("WindowGLFW is already created");
            return;
        }

        GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
        {
            ENGINE_FATAL("GLFW window init failed!");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

        glfwWindowHint(GLFW_SAMPLES, 4);

        if (width > 0 && height > 0)
        {
            // Window mode
            window = glfwCreateWindow(width, height, title, NULL, NULL);
        }
        else
        {
            // Full screen mode
            GLFWmonitor*       monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode    = glfwGetVideoMode(monitor);
            /* Create a windowed mode window and its OpenGL context */
            window = glfwCreateWindow(mode->width, mode->height, title, monitor, NULL);
        }

        if (!window)
        {
            glfwTerminate();
            ENGINE_FATAL("GLFW window create failed!");
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        glfwSwapInterval(vSync);

        GLenum glewErr = glewInit();
        if (glewErr != GLEW_OK)
        {
            ENGINE_FATAL("GLEW init failed! Code %d, Error: %s", glewErr, glewGetErrorString(glewErr));
        }

        glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

        ENGINE_DEBUG("WindowGLFW is created");

        ENGINE_DEBUG("OpenGL version %s", glGetString(GL_VERSION));

        _window = window;

        glfwSetWindowCloseCallback(window, WindowCloseCallback);
    }

    void WindowGLFW::CreateFullScreenWindow(const char* title, bool vSync)
    {
        CreateWindow(0, 0, title, vSync);
    }

    void* WindowGLFW::GetNativeApiWindow() const
    {
        return _window;
    }

    void* WindowGLFW::GetNativeContext()
    {
        return nullptr;
    }

    void WindowGLFW::TickBegin()
    {
        // Poll for and process events
        // glfwPollEvents();

        if (_showDebugWindow && _imGuiAdapter)
        {
            _imGuiAdapter->Imgui_NewFrame();
        }
    }

    void WindowGLFW::TickEnd()
    {
        if (_showDebugWindow && _imGuiAdapter)
        {
            int32_t width;
            int32_t height;
            glfwGetWindowSize(_window, &width, &height);

            _imGuiAdapter->Imgui_EndFrame(width, height);
        }

        // Swap front and back buffers
        glfwSwapBuffers(_window);
    }

    void WindowGLFW::ShowDebugWindow(bool show)
    {
        _showDebugWindow = show;
    }

    void WindowGLFW::AttachImGuiAdapter(IImguiAdapterUPtr adapter)
    {
        _imGuiAdapter = std::move(adapter);
        _imGuiAdapter->Imgui_InitImGuiContext(_window);
    }
} // namespace nador