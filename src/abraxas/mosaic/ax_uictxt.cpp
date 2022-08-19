// ax_uictxt.cpp ------------------------------------------------------------------------------------------------------------------

#include "utils/ax_includes.h"

#include "abraxas/mosaic/ax_uictxt.h" 
#include "abraxas/mosaic/ax_appscreen.h"


 
//---------------------------------------------------------------------------------------------------------------------------------

bool Ax_UICtxt::Init( GLFWwindow *glWindow)
{ 
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL( glWindow, true);

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 410";
    ImGui_ImplOpenGL3_Init(glsl_version);
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_UICtxt::PreRender( Ax_AppScreen* window)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
 
    ImGui::NewFrame();

    // Create the docking environment
    ImGuiWindowFlags  windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                                    ImGuiWindowFlags_NoBackground;

    ImGuiViewport   *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    ImGuiID     dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");

    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_UICtxt::PostRender( Ax_AppScreen* window)
{
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData());

    ImGuiIO &io = ImGui::GetIO();

    if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow    *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent( backup_current_context);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_UICtxt::End( Ax_AppScreen* window)
{
  ImGui_ImplGlfw_Shutdown();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui::DestroyContext();
}

//---------------------------------------------------------------------------------------------------------------------------------
 