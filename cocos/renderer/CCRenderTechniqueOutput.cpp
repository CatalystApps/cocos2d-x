//
//  CCRenderTechniqueOutput.cpp
//  cocos2d_libs
//
//  Created by serhii serhiiv on 10/18/16.
//
//

#include "CCRenderTechniqueOutput.h"
#include "CCGLProgramCache.h"
#include "base/CCConfiguration.h"
#include "renderer/ccGLStateCache.h"

NS_CC_BEGIN

CCRenderTechniqueOutput::CCRenderTechniqueOutput(unsigned int width, unsigned int height,
                                                 unsigned int frame_buffer_id, unsigned int render_buffer_id)
{
    m_frame_width = width;
    m_frame_height = height;
    m_frame_buffer_id = frame_buffer_id;
    m_render_buffer_id = render_buffer_id;
    m_clear_color[0] = 0.f; m_clear_color[1] = 0.f; m_clear_color[2] = 0.f; m_clear_color[3] = 1.f;
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    std::string vertexShaderSource = "varying mediump vec2 v_texcoord;";
#else
    std::string vertexShaderSource = "varying vec2 v_texcoord;";
#endif
    vertexShaderSource += "attribute vec3 a_position; \
    attribute vec2 a_texcoord; \
    void main() \
    { \
    gl_Position = vec4(a_position, 1.0); \
    v_texcoord = a_texcoord; \
    }";
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    std::string fragmentShaderSource = "varying mediump vec2 v_texcoord;";
#else
    std::string fragmentShaderSource = "varying vec2 v_texcoord;";
#endif
    
    fragmentShaderSource += "uniform sampler2D sampler_01; \
    void main() \
    { \
    gl_FragColor = texture2D(sampler_01, v_texcoord); \
    }";
    
    GLuint vs_id = CCRenderTechniqueOutput::compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fs_id = CCRenderTechniqueOutput::compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    m_shader_id = CCRenderTechniqueOutput::linkShader(vs_id, fs_id);
    
    m_samplers[e_shader_sampler_01] = glGetUniformLocation(m_shader_id, "sampler_01");
    m_attributes[e_shader_attribute_position] = glGetAttribLocation(m_shader_id, "a_position");
    m_attributes[e_shader_attribute_texcoord] = glGetAttribLocation(m_shader_id, "a_texcoord");
    
    CCRenderTechniqueOutput::createScreenQuad();
}

CCRenderTechniqueOutput::~CCRenderTechniqueOutput()
{
    
}

void CCRenderTechniqueOutput::createScreenQuad()
{
    m_vertices[0].m_position[0] = -1.f; m_vertices[0].m_position[1] = -1.f; m_vertices[0].m_position[2] = 0.f;
    m_vertices[0].m_texcoord[0] = 0.f; m_vertices[0].m_texcoord[1] = 0.f;
    m_vertices[1].m_position[0] = -1.f; m_vertices[1].m_position[1] = 1.f; m_vertices[1].m_position[2] = 0.f;
    m_vertices[1].m_texcoord[0] = 0.f; m_vertices[1].m_texcoord[1] = 1.f;
    m_vertices[2].m_position[0] = 1.f; m_vertices[2].m_position[1] = -1.f; m_vertices[2].m_position[2] = 0.f;
    m_vertices[2].m_texcoord[0] = 1.f; m_vertices[2].m_texcoord[1] = 0.f;
    m_vertices[3].m_position[0] = 1.f; m_vertices[3].m_position[1] = 1.f; m_vertices[3].m_position[2] = 0.f;
    m_vertices[3].m_texcoord[0] = 1.f; m_vertices[3].m_texcoord[1] = 1.f;

    m_indices[0] = 0;
    m_indices[1] = 1;
    m_indices[2] = 2;
    m_indices[3] = 1;
    m_indices[4] = 2;
    m_indices[5] = 3;
    
    glGenBuffers(1, &m_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_attributes) * 4, m_vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_ibo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 6, m_indices, GL_STATIC_DRAW);
    
    if (cocos2d::Configuration::getInstance()->supportsShareableVAO())
    {
        glGenVertexArrays(1, &m_vao_id);
        glBindVertexArray(m_vao_id);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
        if(m_attributes[e_shader_attribute_position] >= 0)
        {
            glEnableVertexAttribArray(m_attributes[e_shader_attribute_position]);
            glVertexAttribPointer(m_attributes[e_shader_attribute_position], 3, GL_FLOAT, GL_FALSE,
                                  sizeof(vbo_attributes),
                                  (GLvoid*)offsetof(vbo_attributes, m_position));
        }
        if(m_attributes[e_shader_attribute_texcoord] >= 0)
        {
            glEnableVertexAttribArray(m_attributes[e_shader_attribute_texcoord]);
            glVertexAttribPointer(m_attributes[e_shader_attribute_texcoord], 2, GL_FLOAT, GL_FALSE,
                                  sizeof(vbo_attributes),
                                  (GLvoid*)offsetof(vbo_attributes, m_texcoord));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_id);
        
        glBindVertexArray(NULL);
    }
}

unsigned int CCRenderTechniqueOutput::compileShader(const std::string& sourceCode, GLenum shaderType)
{
    GLuint id = glCreateShader(shaderType);
    const GLchar *source = const_cast<GLchar*>(sourceCode.c_str());
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);
    
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        int messageSize = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageSize);
        GLchar *message = new GLchar[messageSize];
        memset(message, 0x0, messageSize * sizeof(GLchar));
        glGetShaderInfoLog(id, messageSize, NULL, message);
        assert(false);
    }
    return id;
}

unsigned int CCRenderTechniqueOutput::linkShader(unsigned int vs_id, unsigned int fs_id)
{
    GLuint id = glCreateProgram();
    glAttachShader(id, vs_id);
    glAttachShader(id, fs_id);
    glLinkProgram(id);
    
    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    
    if(!success)
    {
        int messageSize = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageSize);
        GLchar *message = new GLchar[messageSize];
        memset(message, 0x0, messageSize * sizeof(GLchar));
        glGetShaderInfoLog(id, messageSize, NULL, message);
        assert(false);
    }
    return id;
}

void CCRenderTechniqueOutput::setColorAttachmentTexture(unsigned int color_attachment_texture)
{
    m_color_attachment_texture = color_attachment_texture;
}

void CCRenderTechniqueOutput::setDepthAttachmentTexture(unsigned int depth_attachment_texture)
{
    m_depth_attachment_texture = depth_attachment_texture;
}

void CCRenderTechniqueOutput::resizeFrame(unsigned int width, unsigned int height)
{
    m_frame_width = width;
    m_frame_height = height;
}

void CCRenderTechniqueOutput::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer_id);
    glViewport(0, 0, m_frame_width, m_frame_height);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_STENCIL_TEST);
    
    glClearColor(m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);
    
    GL::useProgram(m_shader_id);
    GL::bindTexture2D(m_color_attachment_texture);
    glUniform1i(m_samplers[e_shader_sampler_01], e_shader_sampler_01);
    
    if (cocos2d::Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(m_vao_id);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
        if(m_attributes[e_shader_attribute_position] >= 0)
        {
            glEnableVertexAttribArray(m_attributes[e_shader_attribute_position]);
            glVertexAttribPointer(m_attributes[e_shader_attribute_position], 3, GL_FLOAT, GL_FALSE,
                                  sizeof(vbo_attributes),
                                  (GLvoid*)offsetof(vbo_attributes, m_position));
        }
        if(m_attributes[e_shader_attribute_texcoord] >= 0)
        {
            glEnableVertexAttribArray(m_attributes[e_shader_attribute_texcoord]);
            glVertexAttribPointer(m_attributes[e_shader_attribute_texcoord], 2, GL_FLOAT, GL_FALSE,
                                  sizeof(vbo_attributes),
                                  (GLvoid*)offsetof(vbo_attributes, m_texcoord));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_id);
    }
}

void CCRenderTechniqueOutput::draw()
{
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
}

void CCRenderTechniqueOutput::unbind()
{
    if (cocos2d::Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(NULL);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
    }
    GL::useProgram(NULL);
}

NS_CC_END
