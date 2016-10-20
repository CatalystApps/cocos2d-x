//
//  CCRenderTechniqueOutput.h
//  cocos2d_libs
//
//  Created by serhii serhiiv on 10/18/16.
//
//

#pragma once

#include "base/CCRef.h"
#include "platform/CCGL.h"
#include "renderer/CCGLProgram.h"

NS_CC_BEGIN

class CC_DLL CCRenderTechniqueOutput : public cocos2d::Ref
{
private:
    
    struct vbo_attributes
    {
        float m_position[3];
        float m_texcoord[2];
    };
    
    enum e_shader_sampler
    {
        e_shader_sampler_01 = 0,
        e_shader_sampler_max
    };
    
    enum e_shader_attribute
    {
        e_shader_attribute_position = 0,
        e_shader_attribute_texcoord,
        e_shader_attribute_max
    };
    
    unsigned int m_frame_width;
    unsigned int m_frame_height;
    unsigned int m_frame_buffer_id;
    unsigned int m_render_buffer_id;
    float m_clear_color[4];
    
    unsigned int m_vbo_id;
    unsigned int m_ibo_id;
    unsigned int m_vao_id;
    unsigned int m_shader_id;
    
    vbo_attributes m_vertices[4];
    unsigned short m_indices[6];
    
    static unsigned int compileShader(const std::string& sourceCode, GLenum shaderType);
    static unsigned int linkShader(unsigned int vs_id, unsigned int fs_id);
    
    void createScreenQuad();
    
    int m_samplers[e_shader_sampler_max];
    int m_attributes[e_shader_attribute_max];
    
protected:
    
    unsigned int m_color_attachment_texture;
    unsigned int m_depth_attachment_texture;
    
public:
    
    CCRenderTechniqueOutput(unsigned int width, unsigned int height,
                            unsigned int frame_buffer_id, unsigned int render_buffer_id);
    ~CCRenderTechniqueOutput();
    
    void setColorAttachmentTexture(unsigned int color_attachment_texture);
    void setDepthAttachmentTexture(unsigned int depth_attachment_texture);
    
    void bind();
    void draw();
    void unbind();
};

NS_CC_END
