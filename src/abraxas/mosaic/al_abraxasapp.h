// ax_abraxasapp.h ------------------------------------------------------------------------------------------------------------------
#pragma once

#include "abraxas/carton/ax_biomeifc.h"  
#include "abraxas/mosaic/ax_appscreen.h" 

//---------------------------------------------------------------------------------------------------------------------------------

class Ax_AbraxasApp
{
    std::string             m_Name; 
    Ax_AppScreen            m_AppScreen;  
    Cy_Atm< uint32_t>       m_SzWorker;

    static Ax_AbraxasApp       *s_Instance;

    static void    ErrorCallback( int error, const char* description);

public:
    Ax_AbraxasApp( const std::string &name) :
        m_Name( name) ,
        m_AppScreen( this),
        m_SzWorker( 0)
    {}
    
    ~Ax_AbraxasApp( void);

    const std::string   &Name( void) const { return m_Name; }

    bool    Init( uint32_t width, uint32_t height);
    
    void    IncrWorker( void) { m_SzWorker.Incr(); }
    void    DecrWorker( void) { m_SzWorker.Decr(); }
    
    static Ax_AbraxasApp   *Instance( void) { return s_Instance; }

    void                RenderLoop();
};

//---------------------------------------------------------------------------------------------------------------------------------
