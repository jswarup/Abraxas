// ax_appscreen.cpp  -----------------------------------------------------------------------------------------------------------------------

#include    "utils/ax_includes.h"
#include    "abraxas/mosaic/ax_appscreen.h"
#include    "abraxas/mosaic/ax_abraxasapp.h"
#include    "abraxas/elems/ax_userinput.h"
 
//---------------------------------------------------------------------------------------------------------------------------------

bool Ax_AppScreen::Init( uint32_t width, uint32_t height)
{
    m_Width = width;
    m_Height = height; 

    // Create the window and store this window as window pointer so that we can use it in callback functions

    m_GLWindow = glfwCreateWindow(  m_Width,  m_Height, m_AbraxasApp->Name().c_str(), nullptr, nullptr); 

    if (!m_GLWindow)
    {
        fprintf(stderr, "Error: GLFW Window couldn't be created\n");
        return false;
    }

    glfwSetWindowUserPointer( m_GLWindow, this);
    m_OGLCtx.Init( m_GLWindow);

    glfwSwapInterval(1); // Enable vsync
 
    m_UICtx.Init( m_GLWindow);
    
    m_SceneView.Init( );
 
    return m_IsRunning;
}

//---------------------------------------------------------------------------------------------------------------------------------

Ax_AppScreen::~Ax_AppScreen()
{
    m_UICtx.End( this); 
    m_OGLCtx.End( m_GLWindow);
}
    
//---------------------------------------------------------------------------------------------------------------------------------

void Ax_AppScreen::OnResize(int width, int height)
{
    m_Width = width;
    m_Height = height;

    m_SceneView.resize( m_Width, m_Height);
    Render();
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_AppScreen::OnScroll(double delta)
{
    m_SceneView.on_mouse_wheel(delta);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_AppScreen::OnKey(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
    }
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_AppScreen::OnClose()
{
    m_IsRunning = false;
}

//---------------------------------------------------------------------------------------------------------------------------------

void   Ax_AppScreen::RenderMainMenuBar( void)
{  
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "CTRL+O"))  
                m_FileBrowser.Invoke( { ".dll", ".db", ".fbx", ".obj", ".pcd", ".pts",}, [&]( const std::string &fName) {
                    return m_PropertyPanel.LoadFile( fName);
                }); 
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) 
            {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) 
            {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) 
            {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) 
            {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) 
            {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::Checkbox("ImGui Demo",&m_ShowImGuiDemo);
            ImGui::Checkbox("ImPlot Demo",&m_ShowImPlotDemo);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
        ImGui::Separator();
    }
     
    return;
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_AppScreen::ShowWorkTree( void)
{
    //ImGui::ShowDemoWindow(&show_demo_window);
    if (ImGui::Begin("", &m_WorkTreeFlg, ImGuiWindowFlags_AlwaysAutoResize))
    {  
        if (ImGui::TreeNode("Object List"))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);
            for (int i = 0; i < 5; i++)
            {
                ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
                bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Name"); 
            }
            ImGui::TreePop();
            ImGui::PopStyleVar();
        }
    }
    ImGui::End();
}
 
//---------------------------------------------------------------------------------------------------------------------------------

void Ax_AppScreen::Render()
{
    // Clear the view
    m_OGLCtx.PreRender( m_GLWindow, m_Width, m_Height);

    // Initialize UI components
    m_UICtx.PreRender( this);

    // Render scene to framebuffer and add it to scene view
    RenderMainMenuBar();

    m_PlotVw.Render();

    m_FileBrowser.Render();
 
    /* 
    if ( m_FileBrowser.HasSelected())
    {
        m_FileName = m_FileBrowser.GetSelected().string();
        m_FileBrowser.Deactivate();

        auto            nameExt = Cy_FStrm::PathNameExt( m_FileName); 
        std::string     extStr = std::get< 1>( nameExt);  
        std::transform( extStr.begin(), extStr.end(), extStr.begin(), [](unsigned char c){ return std::tolower(c); });
        if ( extStr == "dll")
        {
            std::string     dirName = Cy_Aid::DirName( m_FileName);
            std::string     datFile = dirName + "/cw_exub.dat";
            if ( !Cy_FStrm::Exists( datFile))
                m_FileBrowser.Invoke();
            else
            {   
                m_Biome->SetPrefixPath( dirName);
                NodePtr     simLoadTree = m_Biome->FetchSimLoad();
                if ( simLoadTree.IsValid()) 
                    m_PropertyPanel.SetSimLoadTree( simLoadTree);  
            }

        } else if ( extStr == "db") 
        { 
            NodePtr   modelTree = m_Biome->FetchModel( m_FileName);
            if ( modelTree.IsValid()) 
                m_PropertyPanel.SetModelTree( modelTree);  
        } else
        {
            m_SceneView.LoadMesh( m_FileName); 
            m_PropertyPanel.SetMeshTree( m_SceneView.GetMesh()); 
        } 
    } */
    if ( m_ShowImGuiDemo)
        ImGui::ShowDemoWindow(&m_ShowImGuiDemo);
    if ( m_ShowImPlotDemo)
        ImPlot::ShowDemoWindow(&m_ShowImPlotDemo);
    if ( m_PropertyPanel.GetMeshTree())
        m_SceneView.Render();

    m_PropertyPanel.Render();

    // Render the UI 
    m_UICtx.PostRender( this);

    // Render end, swap buffers
    m_OGLCtx.PostRender( m_GLWindow);

    HandleInput();
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_AppScreen::HandleInput()
{
    // TODO: move this and camera to scene UI component?

    if ( glfwGetKey( m_GLWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_SceneView.on_mouse_wheel( -0.4f);
    }

    if ( glfwGetKey( m_GLWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_SceneView.on_mouse_wheel( 0.4f);
    }

    if ( glfwGetKey( m_GLWindow, GLFW_KEY_F) == GLFW_PRESS)
    {
        m_SceneView.reset_view();
    }

    double x, y;
    glfwGetCursorPos( m_GLWindow, &x, &y);

    m_SceneView.on_mouse_move(x, y, Ax_UserInput::GetPressedButton( m_GLWindow));
}

//---------------------------------------------------------------------------------------------------------------------------------


