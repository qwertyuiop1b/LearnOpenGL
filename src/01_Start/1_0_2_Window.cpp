#include "utils/Application.h"


class App: public Application {
public:
    App(unsigned int width, unsigned int height, const std::string& title)
        : Application(width, height, title) {
        init();
    }

    void render() override {
        glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

};

int main() {
    App app(800, 600, "1_00_2_Window");
    app.run();
    return 0;
}