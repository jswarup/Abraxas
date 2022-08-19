// ax_proppanel.h  --------------------------------------------------------------------------------------------------------------
#pragma once

#include    "cyto/silo/cy_treeifc.h"
#include    "abraxas/elems/ax_light.h" 

#include    "abraxas/mosaic/ax_imwidgets.h"
#include    "abraxas/mosaic/ax_iguilets.h"
#include    "abraxas/carton/ax_biomeifc.h"
#include    "abraxas/mosaic/ax_plotvw.h" 

//---------------------------------------------------------------------------------------------------------------------------------

class Ax_AppScreen;
class Ax_Mesh;

//---------------------------------------------------------------------------------------------------------------------------------

struct Ax_TreeDetail : public Cy_TreeIfc
{
    uint32_t                m_PageMax;
    Cy_Bunch< uint32_t>     m_PageStk;
    Cy_Bunch< NodePtr>      m_TreeStack;
    Cy_Bunch< NodePtr>      m_CurTreeSubNodes;

    Ax_TreeDetail( void) :
        m_PageMax( 20)
    {}

    NodePtr     GetCurTree( void) { return m_TreeStack.Size()  ? m_TreeStack.Last() : NULL; }

    void    SetTree( const NodePtr &modelTree)  
    {   
        m_TreeStack.PushBack( modelTree); 
        m_PageStk.PushBack( 0); 
    }

    void        ClearCurTreeSubNodes( void)
    {
        m_CurTreeSubNodes.Clear();
    }

    void        RenderNodePageCounter(  void);
    void        RenderNode(  void);
    void        RenderAttribs( void);
};

//---------------------------------------------------------------------------------------------------------------------------------

class Ax_PropPanel : public Cy_TreeIfc
{ 
    Ax_AppScreen        *m_Screen; 
    bool                m_PropFlg; 
    Ax_Mesh             *m_Mesh;
    Ax_TreeDetail       *m_ActiveTree; 
    std::thread         *m_SimRunTHread;
    Ax_TreeDetail       m_ModelTree;
    Ax_TreeDetail       m_FolioTree;    
    Ax_TreeDetail       m_CodegenTree;      
    Ax_TreeDetail       m_SimBuildTree;     
    Ax_TreeDetail       m_SimLoadTree; 
    Ax_TreeDetail       m_SimRunTree;  
    Ax_TreeDetail       m_ModelDiffTree; 
    Ax_TreeDetail       m_FolioDiffTree;  
    
    bool                m_JacobiFlg;
    uint32_t            m_SzBlkTick;
    std::string         m_FileName;
    Ax_BiomeIfc         m_Biome;

    std::string         m_DiffFileName;
    Cw_BiomeIfc         m_DiffBiome;
    SubDiff             m_ModelDiff;
    SubDiff             m_FolioDiff;

    void        RenderNodePageCounter(  void);
    
    void        RenderTabs( void);

    void        RenderNode( void);
    void        RenderMesh( void);
    void        RenderModel( void);

public:
    Ax_PropPanel( Ax_AppScreen* screen) :
        m_Screen( screen), m_PropFlg( true), m_Mesh( NULL), m_ActiveTree( NULL), m_SimRunTHread( NULL), m_JacobiFlg( false),
        m_SzBlkTick( 1024)
    {}

    NodePtr             ModelTree( void);
    NodePtr             FolidTree( void);
    NodePtr             CodegenTree( void);
    NodePtr             FetchSimBuild( void);
    NodePtr             FetchSimLoad( void);
    NodePtr             FetchSimRun( void);
    
    bool    LoadFile( const std::string &fName);

    bool    LoadDiffFile( const std::string &fName);

    void    SetModelTree( const NodePtr &modelTree)  
    { 
        m_ModelTree.SetTree( modelTree);         
        m_ActiveTree = &m_ModelTree;
    }
 
    void    SetSimLoadTree( const NodePtr &simrunTree)  
    { 
        m_SimLoadTree.SetTree( simrunTree);        
        m_ActiveTree = &m_SimLoadTree;
    } 
    
    
    Ax_Mesh *GetMeshTree( void) const {  return m_Mesh; }
    void    SetMeshTree( Ax_Mesh *mesh) {  m_Mesh = mesh; }
    
    void    Render( void);
    
    void    Render1( void);
    
};

//---------------------------------------------------------------------------------------------------------------------------------
 

