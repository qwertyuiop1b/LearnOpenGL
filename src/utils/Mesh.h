#ifndef OPENGL_UTILS_MESH
#define OPENGL_UTILS_MESH

#include "glm/fwd.hpp"
#include "utils/Shader.h"
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoords;
};

struct Texture2D {
    unsigned int id;
    std::string type;
    std::string path;
};


class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture2D> textures;

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture2D>& textures);

    ~Mesh();

    void draw(Shader& shader);

private:
    unsigned int vao, vbo, ebo;

    void setupMesh();
};


#endif