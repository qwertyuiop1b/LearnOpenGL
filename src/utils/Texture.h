#ifndef OPENGL_UTILS_TEXTURE
#define OPENGL_UTILS_TEXTURE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>


class Texture {
private:
    GLuint textureId;
    int width, height, channels;
    GLenum format;

public:
    Texture();

    ~Texture();

    Texture(const Texture&) = delete;

    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;

    Texture& operator=(Texture&& other) noexcept;

    bool loadFromFile(const std::string& filepath);

    bool createFromData(const unsigned char* data, int w, int h, int ch);

    void bind(GLuint textureUnit = 0) const;

    void unbind() const;

    void setParameter(GLenum parameter, GLuint value);

    void setParameter(GLenum parameter, GLfloat value);

 
    GLuint getId() const { return textureId; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannles() const { return channels; }
    GLenum getFormat() const { return format; }
    bool isValid() const { return textureId != 0; }


};

#endif 