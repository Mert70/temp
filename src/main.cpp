#include <Window.hxx>
#include <TitleBar.hxx>
#include <iostream>
#include <stdexcept>

int main(int argc, char const *argv[])
{
    basic::Window window;
    basic::TitleBar titleBar;
    try
    {
        window.create("TEST", 800, 600);
        titleBar.create(&window);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
    }

    while (window.isRunning())
    {
        window.handleEvents();
        window.beginFrame();

        titleBar.draw();

        window.render();
    }

    return 0;
}
