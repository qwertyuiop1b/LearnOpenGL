#ifndef OPENGL_UTILS_MESH
#define OPENGL_UTILS_MESH

#include "glm/fwd.hpp"
#include "utils/Shader.h"
#include "utils/Texture.h"
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoords;
};



class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    ~Mesh();

    void draw(Shader& shader);

private:
    unsigned int vao, vbo, ebo;

    void setupMesh();
};


#endif