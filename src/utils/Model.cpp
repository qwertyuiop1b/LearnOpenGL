#include "Model.h"
#include "assimp/Importer.hpp"
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/types.h"
#include "glm/fwd.hpp"
#include "utils/Mesh.h"
#include "utils/Texture.h"
#include <cstring>
#include <iterator>
#include <string>
#include <utility>
#include <vector>


Model::Model(const char *path) {
    loadModel(path);
}

void Model::draw(Shader& shader) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader);
    }
}

void Model::loadModel(const char *path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    std::string filepath(path);
    directory = filepath.substr(0, filepath.find_last_of('/'));

    processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }

}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices {};
    std::vector<unsigned int> indices {};
    std::vector<Texture> textures {};

    // vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        if (mesh->HasNormals()) {
            vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }

        if (mesh->HasTextureCoords(0)) {
            vertex.texcoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.texcoords = glm::vec2(0.f, 0.f);
        }

        vertices.push_back(vertex);
    }

    // indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // textures
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
    // textures.insert(textures.end(), std::make_move_iterator(specularMaps.begin()), std::make_move_iterator(specularMaps.end()));
    // std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, TextureType::NORMAL);
    // textures.insert(textures.end(), std::make_move_iterator(normalMaps.begin()), std::make_move_iterator(normalMaps.end()));

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType textureType) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;

        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].getPath().c_str(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            Texture texture;
            texture.setType(textureType);
            texture.loadFromFile(directory + "/" + str.C_Str());

            textures_loaded.push_back(texture);
            textures.push_back(texture);
        }

    }
    return textures;
}