// ui_context.h ------------------------------------------------------------------------------------------------------------------
#pragma once

  
class Ax_AppScreen;

//---------------------------------------------------------------------------------------------------------------------------------

class Ax_UICtxt 
{ 

public: 
    Ax_UICtxt( void)  
    {}
    
    bool  Init(  GLFWwindow *glWindow);

    void  PreRender( Ax_AppScreen * window);

    void  PostRender( Ax_AppScreen * window);

    void  End( Ax_AppScreen * window);
};

//---------------------------------------------------------------------------------------------------------------------------------
 
