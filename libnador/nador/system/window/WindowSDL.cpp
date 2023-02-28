#include <GL/glew.h>
#include <SDL.h>

#include "nador/log/Log.h"
#include "nador/system/window/WindowSDL.h"
#include "nador/utils/ImGuiHelper.h"

namespace nador
{
    WindowSDL::~WindowSDL()
    {
        if (_window)
        {
            SDL_GL_DeleteContext(_context);
            SDL_DestroyWindow(_window);
            SDL_Quit();
        }
    }

    void WindowSDL::CreateWindow(int32_t width, int32_t height, const char* title, bool vSync)
    {
        NADOR_ASSERT(title);

        if (_window == nullptr)
        {
            int32_t sdlInit = SDL_Init(SDL_INIT_VIDEO);
            if (sdlInit < 0)
            {
                ENGINE_FATAL("Failed to initialize SDL video. Code: %d Error: %s", sdlInit, SDL_GetError());
            }

            // Initialize rendering context
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

            uint32_t sdlWindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

            if (width != 0 && height != 0)
            {
                // Simple window
                _window = SDL_CreateWindow(title,
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           width,
                                           height,
                                           sdlWindowFlags);
                if (_window == nullptr)
                {
                    ENGINE_FATAL("Window could not be created! SDL_Error: %s\n", SDL_GetError());
                }
            }
            else
            {
                SDL_DisplayMode DM;
                SDL_GetCurrentDisplayMode(0, &DM);
                width = DM.w;
                height = DM.h;

                // Full screen window
                _window = SDL_CreateWindow(title,
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           width,
                                           height,
                                           sdlWindowFlags);
                if (_window == nullptr)
                {
                    ENGINE_FATAL("Window could not be created! SDL_Error: %s\n", SDL_GetError());
                }

                if (SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN) != 0)
                {
                    ENGINE_FATAL("Window on fullscreen error! SDL_Error: %s\n", SDL_GetError());
                }

                if (SDL_SetWindowDisplayMode(_window, nullptr) != 0)
                {
                    ENGINE_FATAL("Window display mode not set! SDL_Error: %s\n", SDL_GetError());
                }
            }

            _context = SDL_GL_CreateContext(_window);
            if (_context == nullptr)
            {
                ENGINE_FATAL("SDL_GL_CreateContext failed! Error: %s", SDL_GetError());
            }

            // Use Vsync
            if (SDL_GL_SetSwapInterval(vSync) < 0)
            {
                ENGINE_WARNING("Unable to set VSync! SDL Error: %s", SDL_GetError());
            }

            // GLEW Initialize
            GLenum glewErr = glewInit();
            if (glewErr != GLEW_OK)
            {
                ENGINE_FATAL("GLEW init failed! Code %d, Error: %s", glewErr, glewGetErrorString(glewErr));
            }

            ENGINE_DEBUG("SDL Window is created.");
            ENGINE_DEBUG("OpenGL version %s", glGetString(GL_VERSION));
        }
        else
        {
            ENGINE_WARNING("WindowSDL already created!");
        }
    }

    void WindowSDL::CreateFullScreenWindow(const char* title, bool vSync)
    {
        CreateWindow(0, 0, title, vSync);
    }

    void* WindowSDL::GetNativeApiWindow()
    {
        return _window;
    }

    void* WindowSDL::GetNativeContext()
    {
        return _context;
    }

    void WindowSDL::ShowDebugWindow(bool show)
    {
        _showDebugWindow = show;
    }

    void WindowSDL::InitImGui()
    {
        InitImGuiContext(_window, _context);
    }

    void WindowSDL::TickBegin()
    {
        if (_showDebugWindow)
        {
            NewFrameImGui();
        }
    }

    void WindowSDL::TickEnd()
    {
        if (_showDebugWindow)
        {
            int32_t width;
            int32_t height;
            SDL_GetWindowSize(_window, &width, &height);

            EndFrameImGui(width, height);
        }

        /* Swap front and back buffers */
        SDL_GL_SwapWindow(_window);
    }
} // namespace nador