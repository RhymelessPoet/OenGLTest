//
//  VertexShader.h
//  OpenGLTest
//
//  Created by 富贵 on 2021/8/7.
//

#ifndef VertexShader_h
#define VertexShader_h

//const char *vertexShaderStr = "#version 330 core\n"
//    "layout (location = 0) in vec3 aPos;\n"
//    "void main()\n"
//    "{\n"
//    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//    "}\0";

const char *vertexShaderStr = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0\n"
"layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1\n\n"
"out float x;\n"
"uniform float Start;\n"
"out vec3 ourColor; // 向片段着色器输出一个颜色\n\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"    ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色\n"
"    x = aPos.x - Start; \n"
"}\0";

#define VertexShader vertexShaderStr


#endif /* VertexShader_h */
