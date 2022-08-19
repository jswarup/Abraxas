// ax_light.h  -------------------------------------------------------------------------------------------------------------------------
#pragma once  

#include  "abraxas/render/ax_shader.h"
 
//---------------------------------------------------------------------------------------------------------------------------------

class Ax_Light 
{
public:
    glm::vec3   m_Position;
    glm::vec3   m_Color;
    float       m_Strength;

    Ax_Light( void)
    {
        m_Color = glm::vec3(1.0f, 1.0f, 1.0f);
        m_Position = { 1.5f, 3.5f, 3.0f };
        m_Strength = 100.0f;
    }

    ~Ax_Light() {}

    void Update( Ax_Shader *shader) 
    {
        shader->set_vec3( m_Position, "lightPosition");
        shader->set_vec3( m_Color * m_Strength, "lightColor");      
    }
};

//---------------------------------------------------------------------------------------------------------------------------------
 
