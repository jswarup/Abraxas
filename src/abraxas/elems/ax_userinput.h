// ax_userinput.h ------------------------------------------------------------------------------------------------------------------
#pragma once  

#include  "utils/ax_includes.h"
 
//---------------------------------------------------------------------------------------------------------------------------------

class Ax_UserInput
{
public:
    enum ButtonPress  
    {
        Left = 0,
        Right = 1,
        Middle = 2,
        None = 9
    };
    static ButtonPress GetPressedButton( GLFWwindow* window)
    {
        ButtonPress    result = ButtonPress::None;

        if ( glfwGetMouseButton( window, 0) == GLFW_PRESS)
            return Left;
        else if ( glfwGetMouseButton( window, 1) == GLFW_PRESS)
            return Right;
        else if ( glfwGetMouseButton( window, 2) == GLFW_PRESS)
            return Middle;        
        return None;

    }
};

//---------------------------------------------------------------------------------------------------------------------------------
 
