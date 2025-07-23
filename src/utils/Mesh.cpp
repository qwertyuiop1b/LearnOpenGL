#include "Mesh.h"
#include "utils/Texture.h"
#include <glad/glad.h>
#include <string>


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
    this->vertices = std::move(vertices);
    this->indices = std::move(indices);
    this->textures = std::move(textures);

    setupMesh();
}


Mesh::~Mesh() {
    // todo
}

void Mesh::draw(Shader& shader) {
    /*
     * shader code
     * uniform texture_diffuse1
     * uniform texture_diffuse2
     * uniform texture_...
     *
    */

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    std::string name;
    for (unsigned int i = 0; i < textures.size(); i++) {
        auto type = textures[i].getType();
        if (type == TextureType::DIFFUSE) {
            name = "material.diffuse" + std::to_string(diffuseNr++);
        } else if (type == TextureType::SPECULAR) {
            name ="material.spcular" + std::to_string(specularNr++);
        }
        shader.setInt(name, i);
        textures[i].bind(i);
    }
    // reset active texture unit 0
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    // bind vao
    glBindVertexArray(vao);

    // bind vbo and upload data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // bind ebo and upload data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // order: vertex  normal texcoord
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, texcoords));
    glEnableVertexAttribArray(2);

    // todo unbind vbo, ebo buffer;
    glBindVertexArray(0);
}