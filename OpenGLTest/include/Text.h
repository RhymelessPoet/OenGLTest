//
//  Text.hpp
//  OpenGLTest
//
//  Created by 富贵 on 2021/8/16.
//

#ifndef Text_h
#define Text_h

#include <iostream>
#include <vector>
#include <codecvt>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm.hpp"

struct chars{
    char ch;
    GLuint textureID;
};

class Text
{
public:
    Text();
    Text(std::string str);
    Text(std::u32string str32);
    void setTextureAll(GLuint texture);
    void setTexture(unsigned int index, GLuint texture);
    void setTexture(unsigned int beginIndex, unsigned int endIndex, GLuint texture);
    bool hasNext(){ return cur < m_text.size(); };
    chars getNext(){ return m_text[cur++];};
    void reStart(){ cur = 0; };
    
private:
    std::vector<chars> m_text;
    unsigned int cur;
};


#endif /* Text_hpp */
