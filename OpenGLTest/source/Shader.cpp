//
//  Shader.cpp
//  OpenGLTest
//
//  Created by 富贵 on 2021/8/6.
//

#include "Shader.h"

Shader::Shader(const GLchar* vShaderStr, const GLchar* fShaderStr)
{
    init(vShaderStr, fShaderStr);
}

Shader::Shader(const GLchar* vShaderPath, const GLchar* fShaderPath, std::ifstream * shaderFile)
{
    std::string vStr = getShaderStr(vShaderPath);
    std::string fStr = getShaderStr(fShaderPath);
    const GLchar* vShaderStr = vStr.c_str();
    const GLchar* fShaderStr = fStr.c_str();
    
//    std::cout << vShaderStr << std::endl;
//    std::cout << fShaderStr << std::endl;
    init(vShaderStr, fShaderStr);
}

Shader::~Shader()
{
    glDeleteProgram(shaderProgram);
}

void Shader::init(const GLchar *vShaderStr, const GLchar *fShaderStr)
{
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    shaderProgram = glCreateProgram();
    
    if(compileShader(vShaderStr, TAG_SHADER_VERT) && compileShader(fShaderStr, TAG_SHADER_FRAG))
    {
        // create shader program
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragShader);
        glLinkProgram(shaderProgram);
        
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "Error: Program::Linking Failed!" << std::endl;
            return;
        }
        
        //Recycle shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragShader);
    }
    else
        return;
}

std::string Shader::getShaderStr(const GLchar* shaderPath)
{
    std::string shaderCodeStr;
    std::stringstream shaderStream;
    std::ifstream newShaderFile;
    
    newShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    // read shader code string from file
    
    try {
        newShaderFile.open(shaderPath);
        shaderStream << newShaderFile.rdbuf();
//        newShaderFile.clear();
        newShaderFile.close();
//
        shaderCodeStr = shaderStream.str();
//        std::cout << shaderCodeStr << std::endl;
    } catch (std::ifstream::failure e) {
        std::cout << "Error::Shader::FILE_NOT_SUCCESFULLY_READ : " << shaderPath << std::endl;
    }
    
    return shaderCodeStr;
}

void Shader::useShaderProgram()
{
    glUseProgram(shaderProgram);
}

void Shader::setUniform(const std::string &name, bool value)
{
    unsigned int uniformID = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1i(uniformID, value);
}

void Shader::setUniform(const std::string &name, int value)
{
    unsigned int uniformID = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1i(uniformID, value);
}

void Shader::setUniform(const std::string &name, float value)
{
    unsigned int uniformID = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1f(uniformID, value);
}

void Shader::setUniform(const std::string &name, glm::vec3 value)
{
    unsigned int uniformID = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform3f(uniformID, value.x, value.y, value.z);
}

void Shader::setUniform(const std::string &name, glm::vec4 value)
{
    unsigned int uniformID = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform4f(uniformID, value.x, value.y, value.z, value.a);
}

void Shader::setUniform(const std::string &name, glm::mat4 value)
{
    unsigned int uniformID = glGetUniformLocation(shaderProgram, name.c_str());
    glUniformMatrix4fv(uniformID, 1, GL_FALSE, glm::value_ptr(value));
}

bool Shader::compileShader(const char * shaderStr, unsigned int tag)
{
    /*
     glShaderSource函数把要编译的着色器对象作为第一个参数。
     第二参数指定了传递的源码字符串数量，这里只有一个。
     第三个参数是顶点着色器真正的源码，第四个参数我们先设置为NULL
    */
    int success;
    char infoLog[512];
    if(tag == TAG_SHADER_VERT)
    {
        // pass shader source code
        glShaderSource(vertexShader, 1, &shaderStr, NULL);
        // compile shader
        glCompileShader(vertexShader);
        // check shader compile status
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "Error: Shader::Vertex::Compilation Failed!" << std::endl;
            return false;
        }
    }
    else if(tag == TAG_SHADER_FRAG)
    {
        glShaderSource(fragShader, 1, &shaderStr, NULL);
        glCompileShader(fragShader);
        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
            std::cout << "Error: Shader::Fragment::Compilation Failed!" << std::endl;
        }
    }
    else
    {
        std::cout << "Illegal Tag!" << std::endl;
        return false;
    }
    return true;
}
