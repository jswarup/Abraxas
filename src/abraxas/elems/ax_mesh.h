// ax_mesh.h  -------------------------------------------------------------------------------------------------------------------------
#pragma once 
 
#include    "abraxas/elems/ax_axial.h" 
#include    "abraxas/render/ax_glbuffers.h"
 
class Ax_Shader;
//---------------------------------------------------------------------------------------------------------------------------------

class Ax_Mesh  
{
private:
    Ax_VertexIndexBuf               m_VertexIndexBuf;     
    std::vector< Ax_Axial>          m_Vertices;
    std::vector< uint32_t>          m_VertexIndices;
    
public:
    glm::vec3       m_Color = { 1.0f, 0.0f, 0.0f };
    float           m_Roughness = 0.2f;
    float           m_Metallic = 0.1f;

public:
    Ax_Mesh( void) :
        m_Color( { 1.0f, 0.0f, 0.0f }),
        m_Roughness( 0.2f),
        m_Metallic( 0.1f)
    {}

    virtual ~Ax_Mesh( void);

    bool    Load( const std::string &filepath);

    void    AddVertex( const Ax_Axial  &vertex) { m_Vertices.push_back( vertex);  }

    void    AddVertexIndex( unsigned int vertex_idx) { m_VertexIndices.push_back( vertex_idx); }

    const std::vector<unsigned int>   &VertexIndices( void) const { return m_VertexIndices; }

    void    Update( Ax_Shader *shader)  ;
  
    void    Init( void); 
    
    void    Render( void);  
};

//---------------------------------------------------------------------------------------------------------------------------------
 

