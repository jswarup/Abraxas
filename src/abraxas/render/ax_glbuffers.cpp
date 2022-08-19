// opengl_buffermgr.cpp ------------------------------------------------------------------------------------------------------------------

#include "utils/ax_includes.h"
#include "abraxas/render/ax_glbuffers.h"
 
//---------------------------------------------------------------------------------------------------------------------------------

void Ax_VertexIndexBuf::CreateBuffers(const std::vector< Ax_Axial>& vertices, const std::vector<unsigned int>& indices)
{
    glGenVertexArrays( 1, &mVAO);

    glGenBuffers( 1, &mIBO);
    glGenBuffers( 1, &mVBO);

    glBindVertexArray( mVAO);

    glBindBuffer( GL_ARRAY_BUFFER, mVBO);
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( Ax_Axial), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray( 0);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Ax_Axial), (void*)  Ax_Axial::OffPos());

    glEnableVertexAttribArray(1);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Ax_Axial), (void*)  Ax_Axial::OffNormal());

    glBindVertexArray( 0); 
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_VertexIndexBuf::DeleteBuffers()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &mIBO);
    glDeleteBuffers(1, &mVBO);
    glDeleteVertexArrays(1, &mVAO);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_VertexIndexBuf::Bind()
{
    glBindVertexArray(mVAO);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_VertexIndexBuf::Unbind()
{
    glBindVertexArray(0);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_VertexIndexBuf::Draw(int index_count)
{
    Bind();

    // the vertices as line loop
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);

    Unbind();
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_FrameBuf::CreateBuffers(int32_t width, int32_t height)
{
    m_Width = width;
    m_Height = height;

    if ( m_FBO)
    {
        //DeleteBuffers();
    }
    else
        glGenFramebuffers( 1, &m_FBO);
    glBindFramebuffer( GL_FRAMEBUFFER, m_FBO);

    glCreateTextures( GL_TEXTURE_2D, 1, &m_TexId);
    glBindTexture( GL_TEXTURE_2D, m_TexId);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexId, 0);

    glCreateTextures( GL_TEXTURE_2D, 1, &m_DepthId);
    glBindTexture( GL_TEXTURE_2D, m_DepthId);
    glTexStorage2D( GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthId, 0);

    GLenum      buffers[4] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers( m_TexId, buffers);

    Unbind();
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_FrameBuf::DeleteBuffers()
{
    if ( m_FBO)
    {    
        glDeleteTextures(1, &m_TexId);
        glDeleteTextures(1, &m_DepthId);
        glDeleteFramebuffers( GL_FRAMEBUFFER, &m_FBO);
        m_FBO = 0;
        m_TexId = 0;
        m_DepthId = 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_FrameBuf::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glViewport(0, 0, m_Width, m_Height);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_FrameBuf::Unbind( void)
{   
 
    glBindFramebuffer( GL_FRAMEBUFFER, 0); 
}

//---------------------------------------------------------------------------------------------------------------------------------

uint32_t Ax_FrameBuf::GetTexture( void)
{
    return m_TexId;
}

//---------------------------------------------------------------------------------------------------------------------------------
 