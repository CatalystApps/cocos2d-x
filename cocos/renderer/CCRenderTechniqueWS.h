//
//  CCRenderTechniqueWS.h
//  cocos2d_libs
//
//  Created by serhii serhiiv on 10/18/16.
//
//

#pragma once

#include "base/CCRef.h"
#include "platform/CCGL.h"

//#define CC_USE_DEPTH_TEXTURE 1

class CC_DLL CCRenderTechniqueWS : public cocos2d::Ref
{
private:
    
    unsigned int m_frame_width;
    unsigned int m_frame_height;
    unsigned int m_frame_buffer_id;
    float m_clear_color[4];
    
protected:
    
    unsigned int m_color_attachment_texture;
#if defined(CC_USE_DEPTH_TEXTURE)
    unsigned int m_depth_attachment_texture;
#else
    unsigned int m_depth_attachment_buffer;
    unsigned int m_stencil_attachment_buffer;
#endif
    
public:
    
    CCRenderTechniqueWS(unsigned int width, unsigned int height);
    ~CCRenderTechniqueWS();
    
    unsigned int getColorAttachmentTexture() const;
#if defined(CC_USE_DEPTH_TEXTURE)
    unsigned int getDepthAttachmentTexture() const;
#endif
    
    void bind();
    void draw();
    void unbind();
};
