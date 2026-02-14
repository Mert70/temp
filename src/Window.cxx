#include <Window.hxx>
#include <glad/glad.h>
#include <iostream>

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

#include <imgui.h>

namespace basic
{
    bool Window::create(std::string _title, int _width, int _height)
    {
        destroy();
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
        {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
            return false;
        }

        SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;

        float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
        _width = static_cast<int>(static_cast<float>(_width) * main_scale);
        _height = static_cast<int>(static_cast<float>(_height) * main_scale);

#if USE_OPENGL
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        window_flags |= SDL_WINDOW_OPENGL;
#else

#define USE_SDL_RENDERER true

#endif

        m_window = SDL_CreateWindow(_title.c_str(),
                                    _width, _height, window_flags);

        if (!m_window)
        {
            std::cerr << "Failed to create SDL window: " << SDL_GetError() << "\n";
            SDL_Quit();
            return false;
        }
        SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_ShowWindow(m_window);
#if USE_OPENGL
        m_context = SDL_GL_CreateContext(m_window);
        if (!m_context)
        {
            std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << "\n";
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD\n";
            SDL_GL_DestroyContext(m_context);
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }
#else
        m_renderer = SDL_CreateRenderer(m_window, nullptr);
        if (!m_renderer)
        {
            std::cerr << "Failed to create SDL renderer: " << SDL_GetError() << "\n";
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }

#endif

        SDL_SetWindowMinimumSize(m_window, 800, 600);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle &style = ImGui::GetStyle();

        style.ScaleAllSizes(main_scale);
        style.FontScaleDpi = main_scale;
#if USE_OPENGL

        ImGui_ImplSDL3_InitForOpenGL(m_window, m_context);
        ImGui_ImplOpenGL3_Init("#version 330");
#else
        ImGui_ImplSDL3_InitForSDLRenderer(m_window, m_renderer);
        ImGui_ImplSDLRenderer3_Init(m_renderer);
#endif

        m_created = true;
        m_running = true;
        return true;
    }

    void Window::destroy()
    {

        m_running = false;
        if (m_created)
        {

            ImGui_ImplSDL3_Shutdown();
            if (m_context)
            {
                ImGui_ImplOpenGL3_Shutdown();
                SDL_GL_DestroyContext(m_context);
                m_context = nullptr;
            }
            if (m_renderer)
            {
                ImGui_ImplSDLRenderer3_Shutdown();
            }

            ImGui::DestroyContext();

            if (m_renderer)
            {
                SDL_DestroyRenderer(m_renderer);
                m_renderer = nullptr;
            }

            if (m_window)
            {
                SDL_DestroyWindow(m_window);
                m_window = nullptr;
            }

            SDL_Quit();
            m_created = false;
        }
    }

    void Window::handleEvents()
    {
        SDL_Event _e;
        while (SDL_PollEvent(&_e))
        {
            if (_e.type == SDL_EVENT_QUIT)
            {
                m_running = false;
            }
            else if (_e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && _e.window.windowID == SDL_GetWindowID(m_window))
            {
                m_running = false;
            }
            else if (_e.type == SDL_EVENT_WINDOW_RESIZED)
            {
#if USE_OPENGL
                glViewport(0, 0, _e.window.data1, _e.window.data2);
#endif
                m_resized = true;
            }

            ImGui_ImplSDL3_ProcessEvent(&_e);
        }
    }
    int Window::pollEvent(SDL_Event &_e)
    {
        int ret = SDL_PollEvent(&_e);
        if (_e.type == SDL_EVENT_QUIT)
        {
            m_running = false;
        }
        else if (_e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && _e.window.windowID == SDL_GetWindowID(m_window))
        {
            m_running = false;
        }
        else if (_e.type == SDL_EVENT_WINDOW_RESIZED)
        {
#if USE_OPENGL
            glViewport(0, 0, _e.window.data1, _e.window.data2);
#endif
            m_resized = true;
        }

        ImGui_ImplSDL3_ProcessEvent(&_e);
        return ret;
    }

    void Window::showDockSpace()
    {
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin("DockSpace", nullptr, window_flags);
        ImGui::PopStyleVar(2);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        ImGui::End();
    }

    void Window::beginFrame()
    {

#if USE_OPENGL
        ImGui_ImplOpenGL3_NewFrame();
#else
        ImGui_ImplSDLRenderer3_NewFrame();
#endif

        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        showDockSpace();
    }

    void Window::render()
    {

        ImGui::Render();

        ImDrawData *draw_data = ImGui::GetDrawData();
        const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
        if (is_minimized)
        {
            return;
        }

#if USE_OPENGL
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(draw_data);
        SDL_GL_SwapWindow(m_window);

#else

        SDL_SetRenderDrawColor(m_renderer, 114, 144, 154, 255);
        SDL_RenderClear(m_renderer);

        ImGui_ImplSDLRenderer3_RenderDrawData(draw_data, m_renderer);
        SDL_RenderPresent(m_renderer);

#endif

    }
}
