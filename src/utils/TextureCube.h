#ifndef OPENGL_UTILS_TEXTURE_CUBE_H
#define OPENGL_UTILS_TEXTURE_CUBE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

class TextureCube {
public:
    TextureCube();
    ~TextureCube() {
        if (textureId != 0) {
            glDeleteTextures(1, &textureId);
        }
    };

    bool loadFromFiles(const std::vector<std::string>& filepaths);   // right left top bottom back front

    void bind() const {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    };

    void unbind() const {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    GLuint getId() const { return textureId; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }
    GLenum getFormat() const { return format; }

private:
    GLuint textureId;
    int width, height, channels;
    GLenum format;
};


#endif