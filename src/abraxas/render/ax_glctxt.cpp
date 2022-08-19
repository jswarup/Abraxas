// ax_glctxt.cpp ------------------------------------------------------------------------------------------------------------------

#include    "utils/ax_includes.h"
#include    "abraxas/render/ax_glctxt.h"
#include    "abraxas/mosaic/ax_appscreen.h"
 
//---------------------------------------------------------------------------------------------------------------------------------

static void OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto  pWindow = static_cast< Ax_AppScreen*>(glfwGetWindowUserPointer(window));
    pWindow->OnKey(key, scancode, action, mods);
}

//---------------------------------------------------------------------------------------------------------------------------------

static void on_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto  pWindow = static_cast< Ax_AppScreen*>(glfwGetWindowUserPointer(window));
    pWindow->OnScroll(yoffset);
}

//---------------------------------------------------------------------------------------------------------------------------------

static void on_window_size_callback(GLFWwindow* window, int width, int height)
{
    auto  pWindow = static_cast< Ax_AppScreen*>(glfwGetWindowUserPointer(window));
    pWindow->OnResize(width, height);
}

//---------------------------------------------------------------------------------------------------------------------------------

static void on_window_close_callback(GLFWwindow* window)
{
    Ax_AppScreen    *pWindow = static_cast<Ax_AppScreen*>(glfwGetWindowUserPointer(window));
    pWindow->OnClose();
}

//---------------------------------------------------------------------------------------------------------------------------------

bool Ax_GlCtxt::Init( GLFWwindow* glWindow)
{ 
    
    glfwSetKeyCallback( glWindow, OnKeyPress);
    glfwSetScrollCallback( glWindow, on_scroll_callback);
    glfwSetWindowSizeCallback( glWindow, on_window_size_callback);
    glfwSetWindowCloseCallback( glWindow, on_window_close_callback);
    glfwMakeContextCurrent( glWindow);

    GLenum  err = glewInit();
    if (GLEW_OK != err)        // Problem: glewInit failed, something is seriously wrong.      
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return false;
    }

    glEnable( GL_DEPTH_TEST);

    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_GlCtxt::PreRender( GLFWwindow *window, uint32_t w, uint32_t h)
{
    glViewport( 0, 0, w, h);
    glClearColor( 0.2f, 0.2f, 0.2f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_GlCtxt::PostRender( GLFWwindow* glWindow)
{
    glfwPollEvents();
    glfwSwapBuffers( glWindow);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_GlCtxt::End( GLFWwindow *glWindow)
{
    glfwDestroyWindow( glWindow);
    glfwTerminate();
}

//---------------------------------------------------------------------------------------------------------------------------------
 