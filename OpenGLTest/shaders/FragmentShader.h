//
//  FragmentShader.h
//  OpenGLTest
//
//  Created by 富贵 on 2021/8/7.
//

#ifndef FragmentShader_h
#define FragmentShader_h

//const char *fragmentShaderStr = "#version 330 core\n"
//    "out vec4 FragColor;\n"
//    "void main()\n"
//    "{\n"
//    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//    "}\n\0";

const char *fragmentShaderStr = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in float x;\n"
"uniform float Time;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(ourColor * (0.5 - x / 2.0 + x * Time), 0.0f);  //\n"
"}\0";

#define FragmentShader fragmentShaderStr


#endif /* FragmentShader_h */
