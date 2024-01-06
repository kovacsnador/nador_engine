#ifndef __NADOR_WINDOW_SDL_H__
#define __NADOR_WINDOW_SDL_H__

#include <functional>

#include "nador/system/IWindow.h"

struct SDL_Window;

namespace nador
{
    class WindowSDL : public IWindow
    {
        using GLContext_t = void;

    public:
        ~WindowSDL();

        void CreateWindow(int32_t width, int32_t height, const char* title, bool vSync) override;
        void CreateFullScreenWindow(const char* title, bool vSync) override;

        void TickBegin() override;
        void TickEnd() override;

        void* GetNativeApiWindow() const noexcept override;
        void* GetNativeContext() const noexcept override;
        void  ShowDebugWindow(bool show) noexcept override;

        void AttachImGuiAdapter(IImguiAdapterUPtr adapter) override;

    private:
        using WindowDeleter_t  = std::function<void(SDL_Window*)>;
        using ContextDeleter_t = std::function<void(GLContext_t*)>;

        std::unique_ptr<SDL_Window, WindowDeleter_t>   _window { nullptr };
        std::unique_ptr<GLContext_t, ContextDeleter_t> _context { nullptr };

        bool _showDebugWindow;

        IImguiAdapterUPtr _imGuiadapter { nullptr };
    };
} // namespace nador

#endif // !__NADOR_WINDOW_SDL_H__