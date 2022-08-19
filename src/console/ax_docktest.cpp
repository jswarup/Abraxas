// ax_docktest.cpp -----------------------------------------------------------------------------------------------------------------------

#include    "utils/ax_includes.h"

static bool firstLoop = true;
static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

//---------------------------------------------------------------------------------------------------------------------------------

void    ax_DockTest( void)
{ 
}

//---------------------------------------------------------------------------------------------------------------------------------

void    ax_DockTest4( void)
{
    bool* p_open = &firstLoop;
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Example: Simple layout", p_open, ImGuiWindowFlags_MenuBar))
    {
       // IMGUI_DEMO_MARKER("Examples/Simple layout");
       
        ImGui::BeginGroup();
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Close")) *p_open = false;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        static int selected = 0;
        // Left
        {
            ImGui::BeginChild("item view", ImVec2(0, 0)); // Leave room for 1 line below us
            ImGui::Text("MyObject: %d", selected);
            ImGui::Separator();
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Description"))
                {
                    ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Details"))
                {
                    ImGui::Text("ID: 0123456789");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            
        {
            ImGui::BeginChild("left pane", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
            for (int i = 0; i < 100; i++)
            {
                // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
                char label[128];
                sprintf(label, "MyObject %d", i);
                if (ImGui::Selectable(label, selected == i))
                    selected = i;
            }
            ImGui::EndChild();
        }
        //ImGui::SameLine();

        // Right
        {// -ImGui::GetFrameHeightWithSpacing()
            if (ImGui::Button("Revert")) {}
            ImGui::SameLine();
            if (ImGui::Button("Save")) {}
        }
        ImGui::EndChild();
        }
        ImGui::EndGroup();
    }
    ImGui::End();
}

//---------------------------------------------------------------------------------------------------------------------------------

void    ax_DockTest3( void)
{ 
    ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    static bool init = true;
    ImGuiID dock_id_left, dock_id_right;
    if (init) {
        init = false;
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id);
        ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

    	ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.5f, &dock_id_left, &dock_id_right);
        ImGui::DockBuilderDockWindow("Window_1", dock_id_left);
        ImGui::DockBuilderDockWindow("Window_2", dock_id_right);

        ImGui::DockBuilderFinish(dockspace_id);
    }
    ImGui::Begin("Window_1");
    ImGui::Text("Text 1");
    ImGui::End();
    ImGui::Begin("Window_2");
    ImGui::Text("Text 2");
    ImGui::End();
}

//---------------------------------------------------------------------------------------------------------------------------------

void    ax_DockTest2( void)
{ 

    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 0.0f));
    ImGui::Begin("Root", nullptr, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);
    ImGui::End();;

    // Dockspace
    if ( 1) //io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("Root");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        
        static auto first_time = true;
        if (first_time)
        {
            first_time = false;
            
            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags );
            ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);
            
            auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
            auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 1.0f, nullptr, &dockspace_id);
            
            ImGui::DockBuilderDockWindow("Sidebar", dock_id_left);
            ImGui::DockBuilderDockWindow("Content", dock_id_right);
            ImGui::DockBuilderFinish(dockspace_id);
        }
    }


    ImGui::Begin("Sidebar");
    ImGui::Text("Text 1");
    ImGui::Text("Text 2");
    ImGui::Text("Text 3");
    ImGui::End();

    ImGui::Begin("Content");
    ImGui::Button("Button 1");
    ImGui::Button("Button 2");
    ImGui::Button("Button 3");
    ImGui::End();
}

//---------------------------------------------------------------------------------------------------------------------------------

void    ax_DockTest1( void)
{ 
    // Only run DockBuilder functions on the first frame of the app:
    if (firstLoop) {
        // 2. We want our whole dock node to be positioned in the center of the window, so we'll need to calculate that first.
        // The "work area" is the space inside the platform window created by GLFW, SDL, etc. minus the main menu bar if present.
        ImVec2 workPos = ImGui::GetMainViewport()->WorkPos;   // The coordinates of the top-left corner of the work area
        ImVec2 workSize = ImGui::GetMainViewport()->WorkSize; // The dimensions (size) of the work area

        // We'll need to halve the size, then add those resultant values to the top-left corner to get to the middle point on both X and Y.
        ImVec2 workCenter{ workPos.x + workSize.x * 0.5f, workPos.y + workSize.y * 0.5f };

        // For completeness, this can be simplified in modern versions of Dear ImGui.
        // v1.81 (found by git blame) adds a new function GetWorkCenter() which does these same calculations, so for any code using a newer version:
        //
        // if (firstLoop) {
        //     ImVec2 workCenter = ImGui::GetMainViewport()->GetWorkCenter();

        // 3. Now we'll need to create our dock node:
        ImGuiID id = ImGui::GetID("Zero"); // The string chosen here is arbitrary (it just gives us something to work with)
        ImGui::DockBuilderRemoveNode(id);             // Clear any preexisting layouts associated with the ID we just chose
        ImGui::DockBuilderAddNode(id);                // Create a new dock node to use

        // 4. Set the dock node's size and position:
        ImVec2 size{ 600, 300 }; // A decently large dock node size (600x300px) so everything displays clearly

        // Calculate the position of the dock node
        //
        // `DockBuilderSetNodePos()` will position the top-left corner of the node to the coordinate given.
        // This means that the node itself won't actually be in the center of the window; its top-left corner will.
        //
        // To fix this, we'll need to subtract half the node size from both the X and Y dimensions to move it left and up.
        // This new coordinate will be the position of the node's top-left corner that will center the node in the window.
        ImVec2 nodePos{ workCenter.x - size.x * 0.5f, workCenter.y - size.y * 0.5f };

        // Set the size and position:
        ImGui::DockBuilderSetNodeSize(id, size);
        ImGui::DockBuilderSetNodePos(id, nodePos);

        // 5. Split the dock node to create spaces to put our windows in:

        // Split the dock node in the left direction to create our first docking space. This will be on the left side of the node.
        // (The 0.5f means that the new space will take up 50% of its parent - the dock node.)
        ImGuiID dock1 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.5f, nullptr, &id);
        // +-----------+
        // |           |
        // |     1     |
        // |           |
        // +-----------+

        // Split the same dock node in the right direction to create our second docking space.
        // At this point, the dock node has two spaces, one on the left and one on the right.
        ImGuiID dock2 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.5f, nullptr, &id);
        // +-----+-----+
        // |     |     |
        // |  1  |  2  |
        // |     |     |
        // +-----+-----+
        //    split ->

        // For our last docking space, we want it to be under the second one but not under the first.
        // Split the second space in the down direction so that we now have an additional space under it.
        //
        // Notice how "dock2" is now passed rather than "id".
        // The new space takes up 50% of the second space rather than 50% of the original dock node.
        ImGuiID dock3 = ImGui::DockBuilderSplitNode(dock2, ImGuiDir_Down, 0.5f, nullptr, &dock2);
        // +-----+-----+
        // |     |  2  |  split
        // |  1  +-----+    |
        // |     |  3  |    V
        // +-----+-----+

        // 6. Add windows to each docking space:
        ImGui::DockBuilderDockWindow("One", dock1);
        ImGui::DockBuilderDockWindow("Two", dock2);
        ImGui::DockBuilderDockWindow("Three", dock3);

        // 7. We're done setting up our docking configuration:
        ImGui::DockBuilderFinish(id);
    }

    // 8. Create some windows to put in the dock node:
    // (The titles of these windows must match the strings given in DockBuilderDockWindow().)
    ImGui::Begin("One");
    ImGui::End();

    ImGui::Begin("Two");
    ImGui::End();

    ImGui::Begin("Three");
    ImGui::End();

    // 9. At this point, we're at the end of the loop iteration.
    // If this is our first one, set the state variable to false so the DockBuilder functions don't run again:
    if (firstLoop) firstLoop = false;

    // End of main loop
}
