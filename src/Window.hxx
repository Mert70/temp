#ifndef _IHA_BASIC_WINDOW_
#define _IHA_BASIC_WINDOW_

#include <SDL3/SDL.h>
#include <imgui.h>
#include <string>

#define USE_OPENGL true

namespace basic
{
    class Window
    {
    public:
        Window() = default;
        Window(std::string _title, int _width, int _height) { create(_title, _width, _height); }
        ~Window() { destroy(); }

        bool create(std::string _title, int _width, int _height);
        void destroy();

        int pollEvent(SDL_Event &_e);
        void handleEvents();

        void beginFrame();
        void render();
        void showDockSpace();

        SDL_Window *getWindow() { return m_window; }
        SDL_Renderer *getRenderer() { return m_renderer; }

        const SDL_Window *getRWindow() const { return m_window; }
        const SDL_Renderer *getRRenderer() const { return m_renderer; }

        bool isRunning() { return m_running; }

    private:
        bool m_running = false;
        bool m_created = false;
        bool m_resized = true;

        SDL_Window *m_window = nullptr;
        SDL_GLContext m_context = nullptr;
        SDL_Renderer *m_renderer = nullptr;
    };
}

#endif