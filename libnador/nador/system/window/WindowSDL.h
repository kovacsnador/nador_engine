#ifndef __NADOR_WINDOW_SDL_H__
#define __NADOR_WINDOW_SDL_H__

#include <SDL.h>

#include "nador/system/IWindow.h"

class SDL_Window;

namespace nador
{
    class WindowSDL : public IWindow
    {
    public:
        ~WindowSDL();

        void CreateWindow(int32_t width, int32_t height, const char* title, bool vSync) override;
        void CreateFullScreenWindow(const char* title, bool vSync) override;

        void TickBegin() override;
        void TickEnd() override;

        void* GetNativeApiWindow() override;
        void* GetNativeContext() override;
        void  ShowDebugWindow(bool show) override;

        void InitImGui() override;

    private:
        SDL_Window*     _window {nullptr};
        SDL_GLContext   _context {nullptr};

        bool _showDebugWindow;
    };
} // namespace nador

#endif // !__NADOR_WINDOW_SDL_H__