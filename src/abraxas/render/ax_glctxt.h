// opengl_context.h ------------------------------------------------------------------------------------------------------------------
#pragma once
 
class   Ax_AppScreen;

//---------------------------------------------------------------------------------------------------------------------------------

class Ax_GlCtxt 
{ 

public:
    Ax_GlCtxt( void)  
    {}
    
    bool  Init( GLFWwindow* glWindow);

    void  PreRender( GLFWwindow* glWindow, uint32_t w, uint32_t h) ;

    void  PostRender( GLFWwindow* glWindow) ;

    void  End( GLFWwindow* glWindow);
};

//---------------------------------------------------------------------------------------------------------------------------------
 
