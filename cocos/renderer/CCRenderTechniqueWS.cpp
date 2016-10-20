//
//  CCRenderTechniqueWS.cpp
//  cocos2d_libs
//
//  Created by serhii serhiiv on 10/18/16.
//
//

#include "CCRenderTechniqueWS.h"
#include "base/CCConfiguration.h"

CCRenderTechniqueWS::CCRenderTechniqueWS(unsigned int width, unsigned int height)
{
    m_frame_width = width;
    m_frame_height = height;
    m_clear_color = std::make_tuple(0.f, 0.f, 0.f, 1.f);
    
    glGenTextures(1, &m_color_attachment_texture);
    glBindTexture(GL_TEXTURE_2D, m_color_attachment_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_frame_width, m_frame_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    
#if defined(CC_USE_DEPTH_TEXTURE)
    glGenTextures(1, &m_depth_attachment_texture);
    glBindTexture(GL_TEXTURE_2D, m_depth_attachment_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_EXT, GL_COMPARE_REF_TO_TEXTURE_EXT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_EXT, GL_LEQUAL);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_frame_width, m_frame_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL_OES, m_frame_width, m_frame_height, 0, GL_DEPTH_STENCIL_OES, GL_UNSIGNED_INT_24_8_OES, NULL);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    assert(false);
#endif
    
#endif
    
    glGenFramebuffers(1, &m_frame_buffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer_id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment_texture, 0);
    

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(Configuration::getInstance()->supportsOESPackedDepthStencil())
    {
        glGenRenderbuffers(1, &m_depth_attachment_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depth_attachment_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_frame_width, m_frame_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_attachment_buffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth_attachment_buffer);
    }
    else
    {
        glGenRenderbuffers(1, &m_depth_attachment_buffer);
        glGenRenderbuffers(1, &m_stencil_attachment_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depth_attachment_buffer);
        
        if(Configuration::getInstance()->supportsOESDepth24())
        {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, m_frame_width, m_frame_height);
        }
        else
        {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_frame_width, m_frame_height);
        }
        
        glBindRenderbuffer(GL_RENDERBUFFER, m_stencil_attachment_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, m_frame_width, m_frame_height);
        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_attachment_buffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,  GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_stencil_attachment_buffer);
    }
#endif

    
#if defined(CC_USE_DEPTH_TEXTURE)
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment_texture, 0);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment_texture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment_texture, 0);
#endif
    
#else
    
    glGenRenderbuffers(1, &m_depth_attachment_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depth_attachment_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,  m_frame_width, m_frame_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_attachment_buffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth_attachment_buffer);

#endif
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert(status == GL_FRAMEBUFFER_COMPLETE);
}

CCRenderTechniqueWS::~CCRenderTechniqueWS()
{
    
}

unsigned int CCRenderTechniqueWS::getColorAttachmentTexture() const
{
    return m_color_attachment_texture;
}

#if defined(CC_USE_DEPTH_TEXTURE)
unsigned int CCRenderTechniqueWS::getDepthAttachmentTexture() const
{
    return m_depth_attachment_texture;
}
#endif

void CCRenderTechniqueWS::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer_id);
    glViewport(0, 0, m_frame_width, m_frame_height);
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    
    glClearColor(std::get<0>(m_clear_color), std::get<1>(m_clear_color), std::get<2>(m_clear_color), std::get<3>(m_clear_color));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void CCRenderTechniqueWS::draw()
{
    
}

void CCRenderTechniqueWS::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}
