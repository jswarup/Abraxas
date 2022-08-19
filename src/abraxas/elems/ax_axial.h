// vertex_holder.h  -------------------------------------------------------------------------------------------------------------------------
#pragma once 

//---------------------------------------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------------------------------

class Ax_Axial
{
    glm::vec3   m_Pos;
    glm::vec3   m_Normal;

public:
    Ax_Axial() : 
        m_Pos(), m_Normal() 
    {}
  
    Ax_Axial( const glm::vec3 &pos, const glm::vec3 &normal) : 
        m_Pos(pos), m_Normal(normal)
    {}

    ~Ax_Axial() = default;

    const glm::vec3   &Pos( void) const { return m_Pos; }
    const glm::vec3   &Normal( void) const { return m_Normal; }

    static uint64_t   OffPos( void) { return offsetof( Ax_Axial, m_Pos); }
    static uint64_t   OffNormal( void) { return offsetof( Ax_Axial, m_Normal); }

};

//---------------------------------------------------------------------------------------------------------------------------------
 

