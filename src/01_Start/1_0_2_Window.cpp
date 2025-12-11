#include <memory>
#include "utils/Application.h"
#include "utils/Window.h"


class WindowApp{
public:
    WindowApp(unsigned int width, unsigned int height, const std::string& title)
        : window(std::make_unique<Window>(width, height, title)) {
    }

    void run() {
        while (!window->shouldClose()) {
            window->pollEvents();

            // rendering
            glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            window->swapBuffer();
        }
    }

private:
    std::unique_ptr<Window> window;
};

int main() {
    WindowApp app(800, 600, "1_0_2_Window");
    app.run();
    return 0;
}