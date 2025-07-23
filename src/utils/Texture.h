#ifndef OPENGL_UTILS_TEXTURE
#define OPENGL_UTILS_TEXTURE
#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

enum class TextureType: uint8_t {
    DIFFUSE,
    SPECULAR,
    NORMAL,
};

class Texture {
private:
    GLuint textureId;
    int width, height, channels;
    GLenum format;
    TextureType type = TextureType::DIFFUSE;
    std::string path;

public:
    Texture();

    ~Texture();

    Texture(const Texture&) = default;

    Texture& operator=(const Texture&) = default;

    Texture(Texture&& other) noexcept;

    Texture& operator=(Texture&& other) noexcept;

    bool loadFromFile(const std::string& filepath);

    bool createFromData(const unsigned char* data, int w, int h, int ch);

    void bind(GLuint textureUnit = 0) const;

    void unbind() const;

    void setParameter(GLenum parameter, GLuint value);

    void setParameter(GLenum parameter, GLfloat value);

    void setType(TextureType type);

    GLuint getId() const { return textureId; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannles() const { return channels; }
    GLenum getFormat() const { return format; }
    TextureType getType() const { return type; }
    std::string getPath() const { return path; }
    bool isValid() const { return textureId != 0; }


};

#endif