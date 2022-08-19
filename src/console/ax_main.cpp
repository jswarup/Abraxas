// ax_main.cpp -----------------------------------------------------------------------------------------------------------------------

#include    "utils/ax_includes.h"
#include    "abraxas/mosaic/ax_abraxasapp.h"

//---------------------------------------------------------------------------------------------------------------------------------

int main( void)
{
#ifndef CY_DEBUG
    Ax_AbraxasApp  app( "Abraxas");
#else
    Ax_AbraxasApp  app( "Abraxasd");
#endif

    bool    res = app.Init( 1024, 720);
    app.RenderLoop();
    
    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------- 

int main2(int argc, char* argv[])
{ 
    glfwInit(); 
    const char* glslVersion = "#version 460";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    int windowWidth = 1280;
    int windowHeight = 720;
    char fps[10];
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Program", NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); //  vsync 
    //Initialize GLEW + ImGui
    glewInit();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    // Setup Dear ImGui style 
    ImGui::StyleColorsDark();
    ImPlotStyle& ImPlot_Style = ImPlot::GetStyle();
    ImPlot_Style.UseLocalTime = true;
    ImGui::GetStyle().AntiAliasedLines = true;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 21.0f);



    while (!glfwWindowShouldClose(window))
    {

        //Events
        glfwPollEvents();


        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        extern void    ax_DockTest( void);
        ax_DockTest();

        //Fps checker
        sprintf_s(fps, "%.3f", ImGui::GetIO().Framerate);
        glfwSetWindowTitle(window, fps);
       

        // Rendering
        ImGui::Render();
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }




    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();


    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}

//---------------------------------------------------------------------------------------------------------------------------------

int main1(int, char**)
{    
    printf("sample_implot: start\n");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    // Additional imgui initialization needed when no backend is present
    ImGui::GetIO().DisplaySize = ImVec2(400.f, 400.f);
    ImGui::GetIO().Fonts->Build();

    // Render 500 frames
    for(int counter = 0; counter < 500000; ++counter)
    {
        ImGui::NewFrame();

        if (ImGui::Begin("Hello, world!"))
        {
            ImGui::Text("Hello again");

            if (ImPlot::BeginPlot("My Plot"))
            {
                static double values[] = {1., 3., 5.};
                ImPlot::PlotLine("Values", values, 3);
                ImPlot::EndPlot();
            }

            #ifdef IMPLOT_INSTANTIATE_ALL_NUMERIC_TYPES
            if (ImPlot::BeginPlot("My Plot (long double)"))
            {
                static long double values[] = {1., 3., 5.};
                ImPlot::PlotLine("Values", values, 3);
                ImPlot::EndPlot();
            }
            #endif

            ImGui::End();
        }

        ImGui::Render();
    }

    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    printf("sample_implot: end\n");
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------------------

