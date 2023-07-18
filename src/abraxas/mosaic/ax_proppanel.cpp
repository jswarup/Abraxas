// proprety_panel.cpp  ------------------------------------------------------------------------------------------------------------

#include    "utils/ax_includes.h"
#include    "cyto/spritz/cy_flux.h"
#include    "cyto/stalks/cy_chorescheduler.h" 
#include    "abraxas/mosaic/ax_proppanel.h" 
#include    "abraxas/mosaic/ax_abraxasapp.h"
#include    "abraxas/mosaic/ax_appscreen.h"

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_TreeDetail::RenderNodePageCounter(  void)
{  
    uint32_t    *pCounter = m_PageStk.PtrAt( m_PageStk.Size() -1);
    float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##left", ImGuiDir_Left)) 
    {
        ClearCurTreeSubNodes();
        if ( *pCounter >= m_PageMax) 
            *pCounter -= m_PageMax; 
        else 
            *pCounter = 0;
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::ArrowButton("##right", ImGuiDir_Right)) 
    { 
        ClearCurTreeSubNodes();
        uint32_t    newCnt = (*pCounter) +m_PageMax;
        if ( newCnt < m_TreeStack.Last()->SzNode())
            *pCounter = newCnt; 
    }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("%d", *pCounter);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_TreeDetail::RenderNode(  void)
{   
    char    buf[1024];   
    [&]( char *buf, uint32_t sz) {
        for ( uint32_t k = 0; k < m_TreeStack.Size() && sz; ++k)
        { 
            auto            node = m_TreeStack.At( k); 
            const char      *nm = node->Name();
            uint32_t        len = ( uint32_t) strlen( nm);
            if ( sz <= ( len +1))
                break;
            strncpy( buf, nm, len);
            sz -= len +1;
            buf[ len] = '/'; 
            buf += len +1;
            *buf = '\0';
        } 
    }( buf, 1024);

    NodePtr     slctNode;
    
    ImGui::Begin( "Node", NULL, ImGuiWindowFlags_NoCollapse ); 
    ImGui::InputText( "##Path", buf, 64, ImGuiInputTextFlags_CharsNoBlank);
    ImGui::SameLine();
    bool    upclicked =  ImGui::SmallButton("..");
    if ( upclicked && ( m_TreeStack.Size() > 1) )
    { 
        m_TreeStack.PopBack();
        ClearCurTreeSubNodes();
        m_PageStk.PopBack();
    } 
    ImGui::SameLine();
    RenderNodePageCounter();
    int    slctInd = CY_UINT32_MAX;  
    ImGui::SetNextItemWidth(100.0f);
    ImGui::SameLine();
    
    uint32_t    pgBegin = m_PageStk.Last();
    auto        curNode = m_TreeStack.Last();
    uint32_t    szNode = curNode->SzNode();
    if ( ImGui::InputInt("##Slct", &slctInd, 0)  && ImGui::IsItemDeactivated() && ( uint32_t( slctInd) < szNode)) 
        slctNode = curNode->NodeAt( slctInd); 
    ImGui::Separator();
    if ( !slctNode.IsValid() && ImGui::TreeNodeEx( curNode->Name(), ImGuiTreeNodeFlags_DefaultOpen, curNode->Name()))
    {
        ImGui::PushStyleVar( ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);
        
        if ( szNode > ( pgBegin +m_PageMax))
            szNode = pgBegin +m_PageMax; 
        m_CurTreeSubNodes.Resize( szNode -pgBegin, NULL);
        for ( uint32_t i = pgBegin; i < szNode; i++)
        { 
            ImGuiTreeNodeFlags  node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick; 

            auto    subNode = m_CurTreeSubNodes.At( i -pgBegin); 
            if ( !subNode.IsValid())
                subNode = curNode->NodeAt( i);
            uint32_t    nodeKind = m_Biome ? m_Biome->VarKind( subNode)  : 0;
            bool        simVar = ( nodeKind & ( 0x1 << 4 ));
            if ( simVar)
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32( 220,255,235,255));
            bool    node_open = ImGui::TreeNodeEx(  subNode->Name(), node_flags,  "%s[ %u]", subNode->Name(), i);
            if ( simVar)
                ImGui::PopStyleColor();

            if ( node_open && ImGui::IsItemClicked())
            { 
                slctNode = subNode; 
                slctInd = i;
            }
            if (node_open) 
                ImGui::TreePop(); 
        }
        ImGui::TreePop();
        ImGui::PopStyleVar();
    }  
    ImGui::End();
    if ( !upclicked && slctNode.IsValid())
    { 
        m_TreeStack.PushBack( slctNode);
        if ( uint32_t( slctInd) < m_CurTreeSubNodes.Size())
            m_CurTreeSubNodes.SetAt( slctInd, NULL);
        ClearCurTreeSubNodes();
        *m_PageStk.Back() = slctInd;
        m_PageStk.PushBack( 0);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_TreeDetail::RenderAttribs( void)
{   
    //ImGui::Begin( "Attrib", NULL, ImGuiWindowFlags_NoCollapse );

    auto        curNode = m_TreeStack.Last(); 
    uint32_t    szAttr = curNode->SzAttr(); 
    for ( uint32_t i = 0; i < szAttr; ++i)
    {
        auto        attr = curNode->AttrAt( i);
        std::string str = attr.ValueString();
        std::array< char, 20>    bufName;
        const char  *pCh = attr.name;
        for ( uint32_t i = 0; i < bufName.size(); ++i)
            bufName[ i] = *pCh ? *pCh++ : ' ';
            
        ImGui::TextEx( &bufName[ 0], &bufName[ 0] + bufName.size());
        ImGui::SameLine();
        if ( 1)
        { 
            ImGui::TextWrapped( str.c_str(), str.c_str() +str.length(), 0);
        } else
        {
            ImGui::InputTextEx( "##value", NULL, ( char *) str.c_str(), ( int) str.length(),  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 4), ImGuiInputTextFlags_ReadOnly);
        }   
    } 
    //ImGui::End();
}


//---------------------------------------------------------------------------------------------------------------------------------
    
bool    Ax_PropPanel::LoadFile( const std::string &fName)
{
    m_FileName = fName; 

    auto            nameExt = Cy_FStrm::PathNameExt( m_FileName); 
    std::string     extStr = std::get< 1>( nameExt);  
    std::transform( extStr.begin(), extStr.end(), extStr.begin(), [](unsigned char c){ return std::tolower(c); });
    if ( extStr == "dll")
    {
        std::string     dirName = Cy_Aid::DirName( m_FileName);
        std::string     datFile = dirName + "/cw_exub.dat";
        if ( !Cy_FStrm::Exists( datFile))
            return false;
        m_Biome.SetPrefixPath( dirName);
        NodePtr     simLoadTree = m_Biome.FetchSimLoad();
        if ( simLoadTree.IsValid()) 
            SetSimLoadTree( simLoadTree);  
    } else if ( extStr == "db") 
    { 
        NodePtr   modelTree = m_Biome.FetchModel( m_FileName);
        if ( modelTree.IsValid()) 
            SetModelTree( modelTree);  
    } else
    {
        m_Screen->LoadMesh( m_FileName); 
        SetMeshTree( m_Screen->SceneView()->GetMesh()); 
    } 
    return true;;
}

//---------------------------------------------------------------------------------------------------------------------------------
    
bool    Ax_PropPanel::LoadDiffFile( const std::string &fName)
{
    m_DiffFileName = fName; 

    auto            nameExt = Cy_FStrm::PathNameExt( m_DiffFileName); 
    std::string     extStr = std::get< 1>( nameExt);  
    std::transform( extStr.begin(), extStr.end(), extStr.begin(), []( uint8_t c){ return std::tolower(c); });
    if ( extStr != "db") 
        return false;
    
    NodePtr   modelBTree = m_DiffBiome.FetchModel( fName);
    if ( modelBTree.IsValid()) 
    { 
        NodePtr     modelATree = m_ModelTree.GetCurTree(); 
        m_ModelDiff = SubDiff( NodeDiff( modelATree->Name(), 0, modelATree, 0, modelBTree)); 
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------

Cy_TreeIfc::NodePtr Ax_PropPanel::ModelTree( void)
{
    return m_Biome.FetchModel( m_FileName);
}

//---------------------------------------------------------------------------------------------------------------------------------

Cy_TreeIfc::NodePtr Ax_PropPanel::FolioTree( void)
{
    return m_Biome.FetchFolio( m_JacobiFlg);
}

//---------------------------------------------------------------------------------------------------------------------------------

Cy_TreeIfc::NodePtr Ax_PropPanel::CodegenTree( void)
{
    return m_Biome.FetchCodegen( m_SzBlkTick );
}

//---------------------------------------------------------------------------------------------------------------------------------

Cy_TreeIfc::NodePtr Ax_PropPanel::FetchSimBuild( void)
{
    return m_Biome.FetchSimBuild( );
}

//---------------------------------------------------------------------------------------------------------------------------------

Cy_TreeIfc::NodePtr Ax_PropPanel::FetchSimLoad( void)
{
    return m_Biome.FetchSimLoad( );
}

//---------------------------------------------------------------------------------------------------------------------------------

Cy_TreeIfc::NodePtr Ax_PropPanel::FetchSimRun( void)
{
    std::string     scriptPath = "FASLG_TC_ONCO_V1/s.g"; 
    return m_Biome.FetchSimRun( scriptPath, m_Screen->FetchMonitor( m_Biome.FetchSimLoad( )));
}

//---------------------------------------------------------------------------------------------------------------------------------

void        Ax_PropPanel::RenderTabs( void)
{  
    ImGui::SetNextWindowSize( ImVec2( 200, 200));
    ImGui::Begin( "TreeTabs", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_AlwaysAutoResize);

    ImGuiTabBarFlags    tab_bar_flags = ImGuiTabBarFlags_None; 

    Ax_TreeDetail       *activeTree = m_ActiveTree;
    if (ImGui::BeginTabBar( "Layout", tab_bar_flags))
    {
        NodePtr     modelRoot = m_ModelTree.GetCurTree();
        if ( modelRoot.IsValid() && ImGui::BeginTabItem( "Model"))
        {
            if ( !modelRoot.IsValid())
                m_ModelTree.SetTree( modelRoot = ModelTree(), NULL);
            if ( modelRoot.IsValid())
                activeTree = &m_ModelTree;
            ImGui::EndTabItem();
        }
        NodePtr     folioRoot = m_FolioTree.GetCurTree();
        if ( modelRoot.IsValid() && ImGui::BeginTabItem( "Folio"))
        { 
            if ( !folioRoot.IsValid())
                m_FolioTree.SetTree( folioRoot = FolioTree(), &m_Biome);
            if ( folioRoot.IsValid())
                activeTree = &m_FolioTree;
            ImGui::EndTabItem();
        }
        NodePtr   codeGenRoot = m_CodegenTree.GetCurTree();;  
            
        if ( folioRoot.IsValid() && ImGui::BeginTabItem( "Codegen"))
        { 
            if ( !codeGenRoot.IsValid())
                m_CodegenTree.SetTree( codeGenRoot = CodegenTree(), NULL);
            if ( codeGenRoot.IsValid())
                activeTree = &m_CodegenTree;
            ImGui::EndTabItem();
        }
        NodePtr     simBuildRoot = m_SimBuildTree.GetCurTree();
        if ( codeGenRoot.IsValid() && ImGui::BeginTabItem( "SimBuild"))
        {
            if ( !simBuildRoot.IsValid())
                m_SimBuildTree.SetTree( simBuildRoot = FetchSimBuild(), NULL);
            if ( simBuildRoot.IsValid())
                activeTree = &m_SimBuildTree;
            ImGui::EndTabItem();
        } 
        NodePtr     simLoadRoot = m_SimLoadTree.GetCurTree();
        if ( ( simBuildRoot.IsValid() || simLoadRoot.IsValid()) && ImGui::BeginTabItem( "SimLoad"))
        {
            if ( !simLoadRoot.IsValid())
                m_SimLoadTree.SetTree( simLoadRoot = FetchSimLoad(), NULL);
            if ( simLoadRoot.IsValid())
                activeTree = &m_SimLoadTree;
            ImGui::EndTabItem();
        }
        NodePtr     simRunRoot = m_SimRunTree.GetCurTree();
        if ( simLoadRoot.IsValid() && ImGui::BeginTabItem( "SimRun"))
        {
            if ( !simRunRoot.IsValid() && !m_SimRunTHread)
            {
                m_SimRunTHread = new std::thread( [&]( void) {
                    m_Screen->App()->IncrWorker();
                    auto    runTree = FetchSimRun();
                    m_SimRunTree.SetTree(  runTree, NULL);
               });
            }
            if ( simRunRoot.IsValid())
            {  
                activeTree = &m_SimRunTree;
                if ( m_SimRunTHread)
                { 
                    m_SimRunTHread->join();
                    delete m_SimRunTHread;
                    m_SimRunTHread = NULL;
                    m_Screen->App()->DecrWorker();
                }
            }  
            
            ImGui::EndTabItem();
        }
        if ( modelRoot.IsValid()) 
        {   
            NodePtr     modelDiffRoot = m_ModelDiffTree.GetCurTree();
            if ( ImGui::BeginTabItem( "ModelDiff"))
            { 
                if ( !modelDiffRoot.IsValid() && !m_Screen->FileBrowser()->IsActive() && !m_DiffBiome.HasModel()) 
                { 
                    m_Screen->FileBrowser()->Invoke( { ".db", ".iMD",}, [&]( const std::string &fName) {
                        return  LoadDiffFile( fName);                         
                    }); 
                }
                if ( !modelDiffRoot.IsValid() && m_DiffBiome.HasModel())
                {
                    { 
                        Cy_ChoreScheduler       scheduler( m_ServThrds);
                        scheduler.SetOStrm( &std::cout);
                        scheduler.DoInit(); 
                        
                        auto        *queue = scheduler.CurQueue();
                        uint16_t    jobId = queue->Terminal(); 
                        jobId =  queue->Construct( jobId, &m_ModelDiff, &SubDiff::Reduce); 
                        scheduler.CurQueue()->EnqueueJob( jobId);
                        scheduler.DoLaunch();
                    } 
                    m_ModelDiffTree.SetTree( modelDiffRoot = m_ModelDiff.GetNode(), NULL);
                }
                if ( modelDiffRoot.IsValid())
                    activeTree = &m_ModelDiffTree;
                ImGui::EndTabItem();
            } 
            NodePtr     folioDiffRoot = m_FolioDiffTree.GetCurTree();
            if ( ImGui::BeginTabItem( "FolioDiff"))
            {
                if ( !folioDiffRoot.IsValid() && !m_Screen->FileBrowser()->IsActive() && !m_DiffBiome.HasModel()) 
                { 
                    m_Screen->FileBrowser()->Invoke( { ".db", ".iMD",}, [&]( const std::string &fName) {
                        return LoadDiffFile( fName);
                    }); 
                } 
                NodePtr     folioRoot = m_FolioTree.GetCurTree();
                if ( !folioRoot.IsValid())
                    m_FolioTree.SetTree( folioRoot = FolioTree(), &m_Biome);
                
                NodePtr     folioBRoot = m_DiffBiome.FetchFolio();
                if ( !folioDiffRoot.IsValid() && folioBRoot.IsValid())
                {
                    const char *name = modelRoot->Name();
                    m_FolioDiff = SubDiff( NodeDiff( name, 0, folioRoot, 0, folioBRoot));
                    { 
                        Cy_ChoreScheduler       scheduler( m_ServThrds);
                        scheduler.SetOStrm( &std::cout);
                        scheduler.DoInit(); 
                        
                        auto        *queue = scheduler.CurQueue();
                        uint16_t    jobId = queue->Terminal(); 
                        jobId =  queue->Construct( jobId, &m_FolioDiff, &SubDiff::Reduce); 
                        scheduler.CurQueue()->EnqueueJob( jobId);
                        scheduler.DoLaunch();
                    }  
                    m_FolioDiffTree.SetTree( folioDiffRoot = m_FolioDiff.GetNode(), NULL);
                }
                if ( folioDiffRoot.IsValid())
                    activeTree = &m_FolioDiffTree;     
            
                ImGui::EndTabItem();
            } 
        }
        ImGui::EndTabBar(); 
        if ( activeTree)
            m_ActiveTree = activeTree;
        m_ActiveTree->RenderAttribs();
        m_ActiveTree->RenderNode();
    }  
    ImGui::End();
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_PropPanel::RenderModel( void)
{  
    RenderTabs(); 
    //m_ActiveTree->RenderAttribs(); 
    //m_ActiveTree->RenderNode();
    return;
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_PropPanel::RenderMesh( void)
{

    if ( ImGui::CollapsingHeader( "Ax_Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    { 
        if ( ImGui::Button( "Open...")) 
            m_Screen->FileBrowser()->Invoke( { ".fbx", ".obj", ".pcd", ".pts",}, [&]( const std::string &fName) {
                return LoadFile( fName);
            }); 

        ImGui::SameLine(0, 5.0f);
        ImGui::Text( m_Screen->CurrentPath().CStr());
    }

    if ( ImGui::CollapsingHeader( "Material") && m_Mesh)
    {
        ImGui::ColorPicker3( "Color", (float *) &m_Mesh->m_Color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
        ImGui::SliderFloat( "Roughness", &m_Mesh->m_Roughness, 0.0f, 1.0f);
        ImGui::SliderFloat( "Metallic", &m_Mesh->m_Metallic, 0.0f, 1.0f);
    }

    if ( ImGui::CollapsingHeader( "Ax_Light"))
    {
        ImGui::Separator();
        ImGui::Text( "Position");
        ImGui::Separator();
        nimgui::DrawVec3Widget( "Position", m_Screen->SceneView()->get_light()->m_Position, 80.0f);
    }
 
    return;
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_PropPanel::Render( void)
{
     
    if ( m_Mesh)
        RenderMesh();
    else if ( m_ActiveTree)
        RenderModel();   
    return;
}
//---------------------------------------------------------------------------------------------------------------------------------
 