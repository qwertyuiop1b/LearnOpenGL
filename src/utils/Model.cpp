#include "Model.h"



Model::Model(const char *path) {
    loadModel(path);
}

void Model::draw(Shader& shader) {

}

void Model::loadModel(const char *path) {
    
}

void Model::processNode(aiNode *node, const aiScene *scene) {

}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices {};
    std::vector<unsigned int> indices {};
    std::vector<Texture> textures {};
    return Mesh(std::move(vertices), std::move(indices), std::move(textures));
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName) {
    std::vector<Texture> textures;
    return textures;
}