#include "TextureCube.h"
#include <iostream>
#include "stb_define.h"  // Ensure stb_image.h is included correctly

TextureCube::TextureCube() : textureId(0), width(0), height(0), channels(0), format(GL_RGB) {
}

bool TextureCube::loadFromFiles(const std::vector<std::string>& filepaths) {
    if (filepaths.size() != 6) {
        std::cerr << "Error: Exactly 6 file paths are required for a cube map texture." << std::endl;
        return false;
    }

    stbi_set_flip_vertically_on_load(false);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    for (unsigned int i = 0; i < filepaths.size(); ++i) {
        unsigned char* data = stbi_load(filepaths[i].c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cerr << "Failed to load texture: " << filepaths[i] << std::endl;
            std::cerr << "STB Error: " << stbi_failure_reason() << std::endl;
            return false;
        }

        switch (channels) {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
            default:
                std::cerr << "Unsupported number of channels: " << channels << std::endl;
                stbi_image_free(data);
                return false;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return true;
}