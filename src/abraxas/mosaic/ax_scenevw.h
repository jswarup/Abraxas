// ax_scenevw.h  -------------------------------------------------------------------------------------------------------------------------
#pragma once

#include "abraxas/elems/ax_camera.h"
#include "abraxas/elems/ax_mesh.h"
#include "abraxas/elems/ax_light.h"
#include "abraxas/render/ax_shader.h"
#include "abraxas/render/ax_glbuffers.h"
#include "abraxas/elems/ax_userinput.h"

 
//---------------------------------------------------------------------------------------------------------------------------------

class Ax_SceneVw
{
    Ax_Camera       m_Camera;
    Ax_FrameBuf     m_FrameBuffer;
    Ax_Shader       m_Shader;
    Ax_Light        m_Light;
    Ax_Mesh         m_Mesh;
    glm::vec2       m_Size;

public:
    Ax_SceneVw() : 
        m_Size(800, 600)
    {}

    void    Init( void)
    { 
        m_FrameBuffer.Init();
        m_FrameBuffer.CreateBuffers( 800, 600);
        m_Shader.Load( "shaders/vs.shader", "shaders/fs_pbr.shader");
        m_Camera.Init(glm::vec3(0, 0, 3), 45.0f, 1.3f, 0.1f, 100.0f);
    }

    ~Ax_SceneVw()
    {
        m_Shader.unload();
    }

    Ax_Light    *get_light( void) { return &m_Light; }

    void    resize( int32_t width, int32_t height);

    void    Render( void);

    void    LoadMesh( const std::string &filepath); 

    Ax_Mesh *GetMesh( void) { return &m_Mesh; }
    
    void    on_mouse_move( double x, double y, Ax_UserInput::ButtonPress button);

    void    on_mouse_wheel( double delta);

    void    reset_view()
    {
        m_Camera.reset();
    }
};

//---------------------------------------------------------------------------------------------------------------------------------
 
