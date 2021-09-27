//
//  Shader.h
//  OpenGLTest
//
//  Created by 富贵 on 2021/8/6.
//

#ifndef Shader_h
#define Shader_h

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define TAG_SHADER_VERT 0
#define TAG_SHADER_FRAG 1

class Shader
{
public:
    friend class Engine;
    Shader(const GLchar* vShaderStr, const GLchar* fShaderStr);
    Shader(const GLchar* vShaderPath, const GLchar* fShaderPath, std::ifstream * shaderFile);
    ~Shader();
    void setUniform(const std::string &name, bool value);
    void setUniform(const std::string &name, int value);
    void setUniform(const std::string &name, float value);
    void setUniform(const std::string &name, glm::vec3 value);
    void setUniform(const std::string &name, glm::vec4 value);
    void setUniform(const std::string &name, glm::mat4 value);
  
protected:
    void useShaderProgram();
    
private:
    void init(const GLchar* vShaderStr, const GLchar* fShaderStr);
    bool compileShader(const char * shaderStr, unsigned int tag);
    std::string getShaderStr(const GLchar* shaderPath);
    
    //顶点着色器ID
    unsigned int vertexShader;
    //片段(Fragment)着色器
    unsigned int fragShader;
    //着色器程序
    unsigned int shaderProgram;
};

#endif /* Shader_h */
