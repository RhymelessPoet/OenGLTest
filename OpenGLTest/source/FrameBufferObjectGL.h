//
//  FrameBufferObjectGL.hpp
//  OpenGLTest
//
//  Created by 富贵 on 2022/3/16.
//

#ifndef FrameBufferObjectGL_h
#define FrameBufferObjectGL_h
#include<stdint.h>

namespace ht {

enum class AttachmentType{
    Color = 0,
    Depth,
    Stencil,
    Count
};

class FrameBufferObject
{
public:
    static void Create();
    FrameBufferObject();
    ~FrameBufferObject();
    
    void Attach();
private:
     _id;
    uint32_t ss;
};

} // namespace

#endif /* FrameBufferObjectGL_hpp */
