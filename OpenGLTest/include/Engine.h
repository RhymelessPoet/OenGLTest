//
//  Engine.h
//  OpenGLTest
//
//  Created by 富贵 on 2021/8/5.
//

#ifndef Engine_h
#define Engine_h

#include <map>
#include <string>
#include <codecvt>
#include <locale>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Text.h"
#include "Shader.h"

struct Character {
    GLuint textureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    glm::ivec2 Advance;
};

class Engine
{
public:
    Engine();
    Engine(Shader * shader);
    ~Engine();
    bool init(Shader * changeShader);
    void render(unsigned int drawType, float vertices[], int vSize, unsigned int indices[], int iSize, unsigned int &texture);
    void destory();
    
    void initTextRender();
    // direction: 字符排版方向，默认为横排
    void renderText(std::string text, glm::vec3 pos, glm::vec3 color, glm::vec3 direction = glm::vec3(1, 0, 0));
    void renderSDFText(Text * text, glm::vec3 pos, glm::vec4 fontColor, glm::vec3 direction = glm::vec3(1, 0, 1));
    void setFontTexture(const char * fontPath);
    void setImageTexture(const char * imagePath, unsigned int &texture);
    
public:
    const static int Font_Pixel_Size = 100;

private:
    /*
     顶点数组对象：VAO, Vertex Array Object
     顶点缓冲对象：VBO, Vertex Buffer Object
     索引缓冲对象：IBO, Index Buffer Object 或 EBO，Element Buffer Object
    */
    unsigned int VBO;
    unsigned int IBO;
    unsigned int VAO;
    
    Shader * shader;
    
    std::map<GLchar, Character> Characters;
    
};



#endif /* Engine_h */
