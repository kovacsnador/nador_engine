#include <GL/glew.h>

#include "imgui.h"

#ifdef USE_GLFW_WINDOW
#include <GLFW/glfw3.h>
#include "backends/imgui_impl_glfw.h"
#elif USE_SDL_WINDOW
#include <SDL.h>
#include "backends/imgui_impl_sdl.h"
#endif

#include "backends/imgui_impl_opengl3.h"

#include "nador/log/Log.h"
#include "nador/utils/ImGuiHelper.h"

namespace nador
{

    bool ImGuiStrContainerIter(void* vec, int idx, const char** out_text)
    {
        strings_t* vector = reinterpret_cast<strings_t*>(vec);
        if (idx < 0 || (size_t)idx >= vector->size())
        {
            return false;
        }
        *out_text = vector->at(idx).c_str();
        return true;
    }

    bool InitImGuiContext(void* nativeWindow, void* context)
    {
#ifdef  USE_IMGUI
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }


#ifdef USE_GLFW_WINDOW
        if(ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(nativeWindow), true))
        {
            ENGINE_DEBUG("Imgui with GLFW initialized.");
        }
        else
        {
            ENGINE_FATAL("Imgui with GLFW initialize failed!");
        }
#elif USE_SDL_WINDOW
        if(ImGui_ImplSDL2_InitForOpenGL(static_cast<SDL_Window*>(nativeWindow), context))
        {
            ENGINE_DEBUG("Imgui with SDL initialized.");
        }
        else
        {
            ENGINE_FATAL("Imgui with SDL initialize failed!");
        }
#endif

        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui::StyleColorsDark();

        ENGINE_DEBUG("Initialized ImGui.");

        return true;
#else //  USE_IMGUI

        return false;
#endif
    }

    bool NewFrameImGui()
    {
#ifdef  USE_IMGUI
        ImGui_ImplOpenGL3_NewFrame();
#ifdef USE_GLFW_WINDOW
        ImGui_ImplGlfw_NewFrame();
#elif USE_SDL_WINDOW
        ImGui_ImplSDL2_NewFrame();
#endif
        ImGui::NewFrame();

        return true;
#else //  USE_IMGUI
        return false;
#endif
    }

    bool EndFrameImGui(int32_t width, int32_t height)
    {
#ifdef  USE_IMGUI
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)width, (float)height);

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
#ifdef USE_GLFW_WINDOW
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
#elif USE_SDL_WINDOW
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
#endif
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
#ifdef USE_GLFW_WINDOW
            glfwMakeContextCurrent(backup_current_context);
#elif USE_SDL_WINDOW
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
#endif
        }

        return true;
#else //  USE_IMGUI

        return false;
#endif
    }

    bool ShutdownImGui()
    {
#ifdef  USE_IMGUI

        ImGui_ImplOpenGL3_Shutdown();
#ifdef USE_GLFW_WINDOW
        ImGui_ImplGlfw_Shutdown();
#elif USE_SDL_WINDOW
        ImGui_ImplSDL2_Shutdown();
#endif
      
        ImGui::DestroyContext();
        ENGINE_DEBUG("ImGui Shutdown.");

        return true;
#else //  USE_IMGUI

        return false;
#endif
    }

}