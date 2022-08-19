// ax_scenevw.cpp  -------------------------------------------------------------------------------------------------------------------------

#include  "utils/ax_includes.h"
#include  "abraxas/mosaic/ax_scenevw.h" 
 
//---------------------------------------------------------------------------------------------------------------------------------

void  Ax_SceneVw::resize(int32_t width, int32_t height)
{
    m_Size.x = float( width);
    m_Size.y = float( height);
 
    m_FrameBuffer.CreateBuffers((int32_t) m_Size.x, (int32_t) m_Size.y);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_SceneVw::on_mouse_move( double x, double y, Ax_UserInput::ButtonPress button)
{
    m_Camera.on_mouse_move( x, y, button);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_SceneVw::on_mouse_wheel( double delta)
{
    m_Camera.on_mouse_wheel(delta);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_SceneVw::LoadMesh(const std::string& filepath)
{ 
    m_Mesh.Load(filepath); 
}

//---------------------------------------------------------------------------------------------------------------------------------

void  Ax_SceneVw::Render()
{ 
    m_Shader.use();

    m_Light.Update( &m_Shader);

    m_FrameBuffer.Bind();

    if ( m_Mesh.VertexIndices().size())
    {
        m_Mesh.Update( &m_Shader);
        m_Mesh.Render();
    }

    m_FrameBuffer.Unbind(); 
    ImGui::Begin("Scene"); 
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_Size = { viewportPanelSize.x, viewportPanelSize.y };

        m_Camera.set_aspect( m_Size.x / m_Size.y);
        m_Camera.Update( &m_Shader);

        // add rendered texture to ImGUI scene window
        uint64_t      textureID = m_FrameBuffer.GetTexture();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_Size.x, m_Size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }); 
    ImGui::End();
}

//---------------------------------------------------------------------------------------------------------------------------------
 
