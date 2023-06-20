#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use();

    // uniform工具函数
    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setFloat2(const std::string &name, float value1, float value2) const;

    void setFloat3(const std::string &name, float value1, float value2, float value3) const;

    void setFloat3(const std::string &name, glm::vec3 &value) const;

    void setMatrix3(const std::string &name,  glm::mat3 &mat3 ) const;

    void setMatrix4(const std::string &name,  glm::mat4 &mat4 ) const;

};


#endif //OPENGL_SHADER_H
