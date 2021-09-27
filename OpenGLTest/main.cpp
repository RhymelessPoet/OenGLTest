//
//  main.cpp
//  OpenGLTest
//
//  Created by 富贵 on 2021/8/5.
//

#include <iostream>

#include "include/Engine.h"
#include "include/Shader.h"
#include "include/Text.h"


// 初始化glfw的设置
void initAndsetUpWindow();
//define the relation between window and view and
//to be called when the size of window is changed
void frameBufferSizeCallBack(GLFWwindow * window, int width, int height);
//
void processInput(GLFWwindow * window);


int main()
{
    std::cout << "Congratulations You Have Made The First Step！" << std::endl;
    
    /* ---------------------------- Engine Initializing -------------------------------- */
    
    //使用glfw创建窗口之前的一些初始化设置
    initAndsetUpWindow();
    
    //create Window
    GLFWwindow * window = glfwCreateWindow(1280, 720, "TestOpenGL", NULL, NULL);
    
    //check window
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window!" << std::endl;
        //调用glfwTerminate函数释放分配的资源
        glfwTerminate();
        return -1;
    }
    //调用glfwMakeContextCurrent函数设置已创建窗口的Context为当前线程的主Context
    glfwMakeContextCurrent(window);
    
    //Initialize GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<< "Failed to initialize GLOD!" << std::endl;
        return -1;
    }
    
    //to bind view and window to have samilar size changing
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
    
    //set view on window
    // 注意：参数含义与glfwCreateWindow中不同！
//    glViewport(160, 90, 960, 540);

//    float vertices[] = {
//    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
//         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
//         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
//        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
//        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
//    };
//    unsigned int indices[] = {  // note that we start from 0!
//            0, 1, 3,  // first Triangle
//            1, 2, 3   // second Triangle
//        };

    const char * vertexShaderPath = "shaders/LearnOpGL_TextRender_vs";
    const char * fragShaderPath = "shaders/SDFFont_Outline_fs";
    Shader * shader = new Shader(vertexShaderPath, fragShaderPath, NULL);
    
    Engine * engine = new Engine(shader);
    engine->init(NULL);
    
    engine->setFontTexture("resources/55555.ttf");
    engine->initTextRender();
    
    unsigned int textTexture;
//    engine->setImageTexture("resources/awesomeface.png", textTexture);
    engine->setImageTexture("resources/container.jpg", textTexture);
    
    Text * text = new Text(U"a买这是中文");
    text->setTextureAll(textTexture);
    
    /* ---------------------------- Render Looping -------------------------------- */
    
    //create loop by glfw
    while(!glfwWindowShouldClose(window))
    {
        //be looping before window closed
        
        //process the input from .....
        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //engine->render(GL_TRIANGLES, vertices, sizeof(vertices), indices, sizeof(indices), texture);
        
//        engine->renderText("You are great!", glm::vec3(25.0f, 25.0f, 1.0f), glm::vec3(0.5, 0.8f, 0.2f), glm::vec3(1.0, 1.0, 1.0));
//        engine->renderText("Thanks, You are also great!", glm::vec3(540.0f, 570.0f, 1.5f), glm::vec3(0.3, 0.7f, 0.9f), glm::vec3(-0.0, -1.0, 1.0));
//        engine->renderText("3.141592653 is PI", glm::vec3(800.0f, 200.0f, 0.5f), glm::vec3(1.0f, 0.5f, 0.1f), glm::vec3(1.0, 0.0, 1.0));
//        engine->renderText("The water of the Yellow River comes from the sky.", glm::vec3(10.0f, 350.0f, 0.5f), glm::vec3(.0f, 0.1f, 1.0f), glm::vec3(1.0, -1.0, 1.0));
        
        engine->renderSDFText(text, glm::vec3(10.0f, 100.0f, 600.0f), glm::vec4(0.2f, 0.6f, 0.9f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        
        //update window screen
        glfwSwapBuffers(window);
        /*
         Double Buffer: 前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。
           当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓冲.
        */
        
        //Event poll 轮询
        glfwPollEvents();
        
    }
    
    
    /* ---------------------------- Engine destroy -----------------------------*/
    glfwTerminate();
    
    return 0;
}

void initAndsetUpWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    //for Mac OS X
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void frameBufferSizeCallBack(GLFWwindow * window, int width, int height)
{
    std::cout << width << "   " << height << std::endl;
    int view_x = width / 8;
    int view_y = height / 8;
    int view_width = view_x * 6;
    int view_height = view_y * 6;
    glViewport(view_x, view_y, view_width, view_height);
//    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
