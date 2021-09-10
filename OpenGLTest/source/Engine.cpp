//
//  Engine.cpp
//  OpenGLTest
//
//  Created by 富贵 on 2021/8/5.
//

#include "Engine.h"

#include <iostream>
#include <cmath>
#include "stb_image.h"

Engine::Engine()
{
    // 第一个参数：代表创建VAO的数量
    glGenVertexArrays(1, &VAO);
    // 第一个参数：代表创建BUFFER的数量
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
}

// !!!!!!!!! 不要在有参构造函数里调用其它构造函数，以参数列表的方式除外！！！
Engine::Engine(Shader * shader):Engine()
{
    this->shader = shader;
}

Engine::~Engine()
{
    destory();
}
bool Engine::init(Shader * changeShader)
{
    if(changeShader == NULL)
    {
        if(shader == NULL)
        {
            std::cout << "Error: Engine::NO_SHADER !" << std::endl;
            return false;
        }
    }
    else
    {
        shader = changeShader;
        std::cout << "Log: Engine::Changed_Shader !" << std::endl;
    }
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//
//    // 以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
//    glEnableVertexAttribArray(0);
    
    return false;
}

void Engine::setFontTexture(const char *fontPath)
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    shader->useShaderProgram();
    glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f);
    shader->setUniform("proj", projection);
    
    FT_Library ft;
    if(FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    
    FT_Face face;
    if(FT_New_Face(ft, fontPath, 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 256);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 禁用字节对齐限制
    
    std::u16string chars = u"一人口大呈现藏";
    uint32_t c;
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    for(uint32_t i = 0; i < 1280; i++)
    {
        if(i < 7){
            c = chars[i];
        } else{
            c = i;
        }
        std::cout << i << ": " << chars[i] << std::endl;
//        if(c == '@' || c == 'W'|| c == '&' || c == '>' || c == '^' || c == 'n'){
//            continue;
//        }
       
        // 加载字符的字形 glyph
        if(FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        std::chrono::duration<double> timeLoaded = std::chrono::steady_clock::now() - start;
        std::cout << "Load Time is " << timeLoaded.count() << std::endl;
//        std::cout << "before rendering: " << face->glyph->bitmap.rows << std::endl;
        if(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_SDF)){
            std::cout << "ERROR::FREETYTPE: Failed to render SDF" << std::endl;
            continue;
        }
        std::chrono::duration<double> timeUsed = std::chrono::steady_clock::now() - start;
        std::cout << "Render Time is " << timeUsed.count() << std::endl;
//        std::cout << "after rendering: " << face->glyph->bitmap.rows << std::endl;
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        GLuint width = face->glyph->bitmap.width;
        GLuint height = face->glyph->bitmap.rows;
        FT_Byte* data = (FT_Byte*)face->glyph->bitmap.buffer;
//        for(int i=0; i < height; ++i){
//            for(int j=0; j < width; ++j){
//                printf("%x ", data[i * width + j]);
//            }
//            std::cout << std::endl;
//        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);

        // 设置纹理选项
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        Character charx = {
            texture,
            glm::vec2(width, height),
            glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            glm::vec2(face->glyph->advance.x, face->glyph->advance.y)
        };
        Characters.insert(std::pair<GLchar, Character>(c, charx));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Engine::initTextRender()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}



void Engine::renderText(Text * text, glm::vec3 pos, glm::vec3 direction)
{
    shader->useShaderProgram();
    
    float timeValue = glfwGetTime();
    float Time = sin(timeValue / 1.5f) / 2.0f + 0.5f;
    shader->setUniform("Time", Time);
    
    shader->setUniform("texTure", 0);
    shader->setUniform("textureColor", 1);
    
    glBindVertexArray(VAO);
    
    unsigned int line_height = 0;
    unsigned int word_posx = pos.x;
    text->reStart();
    while (text->hasNext())
    {
        chars c = text->getNext();
        Character ch = Characters[c.ch];
        
        GLfloat scale = pos.z;
        GLfloat xpos = pos.x + ch.Bearing.x * scale;
        GLfloat ypos = pos.y - (ch.Size.y - ch.Bearing.y) * scale;
        
        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h, 0.0, 0.0},
            { xpos,     ypos,     0.0, 1.0},
            { xpos + w, ypos,     1.0, 1.0},
            
            { xpos,     ypos + h, 0.0, 0.0},
            { xpos + w, ypos,     1.0, 1.0},
            { xpos + w, ypos + h, 1.0, 0.0},
        };
        //
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, c.textureID);
        //std::cout << glGetError() << std::endl;
        // 更新VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //
        if(c.ch == 32) // space
        {
            if(direction.x <= 0)
            {
                word_posx += direction.x * (ch.Advance.x >> 6) * scale;
                pos.x = word_posx;
            }
            else
            {
                pos.x += direction.x * (ch.Advance.x >> 6) * scale;
                word_posx = pos.x;
            }
            
            pos.y += direction.y * line_height * scale;
            pos.z *= direction.z;
        }
        else
        {
            //
            line_height = line_height > ch.Size.y ? line_height : ch.Size.y;
            pos.x += (ch.Advance.x >> 6) * scale;
        }
    }
    glBindVertexArray(0);
}

void Engine::renderText(std::string text, glm::vec3 pos, glm::vec3 color, glm::vec3 direction)
{
    shader->useShaderProgram();
    shader->setUniform("textColor", color);
    
    float timeValue = glfwGetTime();
    float Time = sin(timeValue / 1.5f) / 2.0f + 0.5f;
    shader->setUniform("Time", Time);
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    
    std::string::const_iterator c;
    unsigned int line_height = 0;
    unsigned int word_posx = pos.x;
    for(c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];
        
        GLfloat scale = pos.z;
        GLfloat xpos = pos.x + ch.Bearing.x * scale;
        GLfloat ypos = pos.y - (ch.Size.y - ch.Bearing.y) * scale;
        
        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h, 0.0, 0.0},
            { xpos,     ypos,     0.0, 1.0},
            { xpos + w, ypos,     1.0, 1.0},
            
            { xpos,     ypos + h, 0.0, 0.0},
            { xpos + w, ypos,     1.0, 1.0},
            { xpos + w, ypos + h, 1.0, 0.0},
        };
        
        //
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        // 更新VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //
        if(*c == 32) // space
        {
            if(direction.x <= 0)
            {
                word_posx += direction.x * (ch.Advance.x >> 6) * scale;
                pos.x = word_posx;
            }
            else
            {
                pos.x += direction.x * (ch.Advance.x >> 6) * scale;
                word_posx = pos.x;
            }
            
            pos.y += direction.y * line_height * scale;
            pos.z *= direction.z;
        }
        else
        {
            //
            line_height = line_height > ch.Size.y ? line_height : ch.Size.y;
            pos.x += (ch.Advance.x >> 6) * scale;
        }
    }
    glBindVertexArray(0);
//    glBindTexture(GL_TEXTURE_2D, 0);
}

void Engine::setImageTexture(const char *imagePath, unsigned int &texture)
{
    // 生成并初始化纹理对象
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // 设置纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // 设置多级纹理过滤（插值）方式
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // 加载纹理图像
    int width, height, nChannels;
    unsigned char * data = stbi_load(imagePath, &width, &height, &nChannels, 0);
//    std::cout << width << " " << height << std::endl;
    if(data)
    {
        /*
         第一个参数指定了纹理目标(Target).
            设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D
            和GL_TEXTURE_3D的纹理不会受到影响）。
         第二个参数为纹理指定多级渐远纹理的级别.
            如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
         第三个参数告诉OpenGL我们希望把纹理储存为何种格式。
            我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
         第四个和第五个参数设置最终的纹理的宽度和高度。
            我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
            下个参数应该总是被设为0（历史遗留的问题）。
         第七第八个参数定义了源图的格式和数据类型。
            我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
         最后一个参数是真正的图像数据。
         */
        // 配置纹理属性
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // 生成多级纹理
//        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    // 释放图像内存
    stbi_image_free(data);
}


void Engine::render(unsigned int drawType, float vertices[], int vSize, unsigned int indices[], int iSize, unsigned int &texture)
{
    // Bind Buffer
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    
    //pass Data
    glBufferData(GL_ARRAY_BUFFER, vSize, vertices, GL_STREAM_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indices, GL_STREAM_DRAW);
    
    
    /*
     glVertexAttribPointer函数的参数非常多，所以我会逐一介绍它们：
     第一个参数指定我们要配置的顶点属性。
        还记得我们在顶点着色器中使用layout(location=0)定义了position顶点属性的位置值(Location)吗？
        它可以把顶点属性的位置值设置为0。
        因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
     第二个参数指定顶点属性的大小。
        顶点属性是一个vec3，它由3个值组成，所以大小是3。
     第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
     第四个参数定义我们是否希望数据被标准化(Normalize)。
        如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。
        我们把它设置为GL_FALSE。
     第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。
        由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。
        要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）
        我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。
        一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子
       （译注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节）。
     第六个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。
        它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。
     */
    
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    shader->useShaderProgram();
    shader->setUniform("ourTexture", 0);
    shader->setUniform("Start", -0.5f);
    float timeValue = glfwGetTime();
    float Time = sin(timeValue / 1.5f) / 2.0f + 0.5f;
//    std::cout << Time << std::endl;
    shader->setUniform("Time", Time);
    
    // seeing as we only have a single VAO there's no need to bind it every time,
    // but we'll do so to keep things a bit more organized
    glBindVertexArray(VAO);
    
    /*
       第一个参数指定了我们绘制的模式，这个和glDrawArrays的一样。
       第二个参数是我们打算绘制顶点的个数，这里填6，也就是说我们一共需要绘制6个顶点。
       第三个参数是索引的类型，这里是GL_UNSIGNED_INT。
       最后一个参数里我们可以指定EBO中的偏移量
          （或者传递一个索引数组，但是这是当你不在使用索引缓冲对象的时候），
           但是我们会在这里填写0。
     */
    glDrawElements(drawType, 6, GL_UNSIGNED_INT, 0);
    
}

void Engine::destory()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}
