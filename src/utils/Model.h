#ifndef OPENGL_UTILS_MODEL
#define OPENGL_UTILS_MODEL
#include <string>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "utils/Mesh.h"
#include "utils/Shader.h"
#include "utils/Texture.h"


class Model {
public:
    Model(const char *path);

    void draw(Shader &shader);
private:
    std::string directory;
    std::vector<Mesh> meshes;
    std::vector<Texture2D> textures_loaded;

    void loadModel(const char *path);

    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture2D> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName);
};

#endif