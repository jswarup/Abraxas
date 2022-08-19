// ax_appscreen.h  -------------------------------------------------------------------------------------------------------------------------
#pragma once

//---------------------------------------------------------------------------------------------------------------------------------

#include "abraxas/elems/ax_mesh.h"
#include "abraxas/elems/ax_camera.h"
#include "abraxas/elems/ax_light.h"
#include "abraxas/render/ax_shader.h"

#include "abraxas/mosaic/ax_uictxt.h"
#include "abraxas/render/ax_glctxt.h"
#include "abraxas/render/ax_glbuffers.h" 

#include "abraxas/mosaic/ax_proppanel.h"
#include "abraxas/mosaic/ax_scenevw.h"  
#include  "abraxas/mosaic/ax_plotvw.h" 
 
class Ax_BiomeIfc;
class Ax_AbraxasApp;

//---------------------------------------------------------------------------------------------------------------------------------

class Ax_AppScreen : public Cy_TreeIfc
{
    typedef  bool   (Monitor)( uint32_t entInd, double time, uint32_t speciesInd, double speciesValue, double clusterValue, uint32_t sz);
    
    Ax_AbraxasApp              *m_AbraxasApp;
    GLFWwindow              *m_GLWindow;

// Render contexts
    Ax_UICtxt               m_UICtx;
    Ax_GlCtxt               m_OGLCtx;

// UI components
    Ax_IGFilebrowzer        m_FileBrowser;
    Ax_PropPanel            m_PropertyPanel;
    Ax_SceneVw              m_SceneView; 
    Ax_PlotVw               m_PlotVw; 
    bool                    m_IsRunning;
    std::string             m_CurrPath; 

    bool                m_ShowImGuiDemo = true;  
    bool                m_ShowImPlotDemo = true;  
    
    bool             m_WorkTreeFlg;
    uint32_t         m_Width;
    uint32_t         m_Height; 
public: 

    Ax_AppScreen( Ax_AbraxasApp *app) :
        m_AbraxasApp( app),
        m_GLWindow( nullptr),
        m_FileBrowser( "Open.."), 
        m_PropertyPanel( this),
        m_IsRunning( true), 
        m_WorkTreeFlg( true)
    {}

    ~Ax_AppScreen( void);

    bool    Init( uint32_t width, uint32_t height );

    Ax_AbraxasApp          *App( void) const { return m_AbraxasApp; } 

    void LoadMesh( const std::string &fName) { m_SceneView.LoadMesh( fName);  }

    void    RenderMainMenuBar( void);
      
    void    Render( void);

    void    HandleInput( void);

    Cy_CStr CurrentPath( void)   { return m_CurrPath; }
    
    Ax_SceneVw          *SceneView( void) { return &m_SceneView; }
    Ax_PlotVw           *PlotVw( void) { return &m_PlotVw; }
    Ax_IGFilebrowzer    *FileBrowser( void)  { return &m_FileBrowser; }
 
    auto    FetchMonitor( const NodePtr &layout)  { return m_PlotVw.FetchMonitor( layout); }

    void    OnScroll( double delta)  ;

    void    OnKey( int key, int scancode, int action, int mods)  ;

    void    OnResize( int width, int height)  ;

    void    OnClose( void)  ;

    bool    IsRunning( void) { return m_IsRunning; }
    
    void    ShowWorkTree( void);

};

//---------------------------------------------------------------------------------------------------------------------------------
 


