#include "utils/OribitCamera.h"
#include "utils/Window.h"
#include "utils/Shader.h"
#include "utils/Texture.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"
#include <memory>
#include <string>
#include <vector>


class SpecularMap {
public:
    SpecularMap(unsigned int width, unsigned int height, const std::string& title)
    : window(std::make_unique<Window>(width, height, title))
    , diffuseTex(std::make_unique<Texture>("assets/textures/container2.png"))
    , specularTex(std::make_unique<Texture>("assets/textures/container2_specular.png"))
    , cubeShader(std::make_unique<Shader>("shaders/02_shaders/2_4_2_SpecularMap.vert", "shaders/02_shaders/2_4_2_SpecularMap.frag"))
    , lightShader(std::make_unique<Shader>("shaders/02_shaders/2_1_1_Light.vert", "shaders/02_shaders/2_1_1_Light.frag"))
    , cubeVao(std::make_unique<VertexArray>())
    , lightVao(std::make_unique<VertexArray>())
    , orbitCamera(std::make_unique<OribitCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), 10.f, 1.f, glm::radians(90.f), glm::radians(45.f)))
    {
        init();
    }
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Texture> diffuseTex;
    std::unique_ptr<Texture> specularTex;
    std::unique_ptr<Shader> cubeShader;
    std::unique_ptr<Shader> lightShader;
    std::unique_ptr<VertexArray> cubeVao;
    std::unique_ptr<VertexArray> lightVao;
    std::unique_ptr<OribitCamera> orbitCamera;

    void init() {
        
    }
};


int main() {
    SpecularMap app(800, 600, "2.4.2.SpecularMap");
    return 0;
}