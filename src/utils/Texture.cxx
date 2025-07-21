#include "Texture.h"
#include <iostream>
// Include 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(): textureId(0), width(0), height(0), channels(0), format(GL_RGB) {

}

Texture::~Texture() {
    if (textureId != 0) {
        glDeleteTextures(1, &textureId);
    }
}

Texture::Texture(Texture&& other) noexcept : textureId(other.textureId), width(other.width), height(other.height), channels(other.channels), format(other.format){
    other.textureId = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (textureId != 0) {
            glDeleteTextures(1, &textureId);
        }
        textureId = other.textureId;
        width = other.width;
        height = other.height;
        channels = other.channels;
        format = other.format;
        other.textureId = 0;
    }
    return *this;
}

bool Texture::loadFromFile(const std::string& filepath) {
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << filepath << std::endl;
        std::cerr << "STB Error: " << stbi_failure_reason() << std::endl;
        return false;
    }

    switch(channels) {
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

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Free image data
    stbi_image_free(data);
    
    std::cout << "Texture loaded successfully: " << filepath << " (" << width << "x" << height << ", " << channels << " channels)" << std::endl;
    return true;
}


bool Texture::createFromData(unsigned char* data, int w, int h, int ch) {
    // Create texture from raw data
    if (!data) {
        std::cerr << "Invalid data provided to createFromData" << std::endl;
        return false;
    }
    
    width = w;
    height = h;
    channels = ch;
    
    // Determine format
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
            return false;
    }
    
    // Generate and bind texture
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return true;
}


void Texture::bind(GLuint textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}


// Set texture parameters
void Texture::setParameter(GLenum parameter, GLint value) {
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, parameter, value);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setParameter(GLenum parameter, GLfloat value) {
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, parameter, value);
    glBindTexture(GL_TEXTURE_2D, 0);
}
