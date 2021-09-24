//
//  Text.cpp
//  OpenGLTest
//
//  Created by 富贵 on 2021/8/16.
//

#include "Text.h"

Text::Text(std::u32string str32)
{
    cur = 0;
    for(auto c : str32){
        chars ch;
        ch.ch = c;
        m_text.push_back(ch);
    }
}

Text::Text(std::string str)
{
    cur = 0;
    for(auto c : str){
        chars ch;
        ch.ch = c;
        m_text.push_back(ch);
    }
}

void Text::setTextureAll(GLuint textureID)
{
    for(auto& e: m_text)
    {
        e.textureID = textureID;
    }
}
