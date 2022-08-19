// ax_glbuffers.h ------------------------------------------------------------------------------------------------------------------
#pragma once
 
#include "abraxas/elems/ax_axial.h"
 
//---------------------------------------------------------------------------------------------------------------------------------

class Ax_VertexIndexBuf 
{
    uint32_t    mVBO;
    uint32_t    mVAO;
    uint32_t    mIBO;

public:
    Ax_VertexIndexBuf()   : 
      mVBO( 0), mVAO( 0), mIBO( 0)
    {}

    void      CreateBuffers(const std::vector< Ax_Axial>& vertices, const std::vector<unsigned int>& indices);

    void      DeleteBuffers();

    void      Bind();

    void      Unbind();

    void      Draw(int index_count);

};

//---------------------------------------------------------------------------------------------------------------------------------

class Ax_FrameBuf 
{
    uint32_t    m_FBO;
    uint32_t    m_TexId;
    uint32_t    m_DepthId;
    int32_t     m_Width;
    int32_t     m_Height;
public:
    Ax_FrameBuf( void) 
    {}
    
    void Init( void)
    {
        m_FBO = 0;
        m_TexId = 0;
        m_DepthId = 0;
        m_Width = 0;
        m_Height = 0;
    }

    void      CreateBuffers( int32_t width, int32_t height) ;

    void      DeleteBuffers(  void);

    void      Bind( void);

    void      Unbind( void);
    
    uint32_t  GetTexture( void);
};

//---------------------------------------------------------------------------------------------------------------------------------
 
