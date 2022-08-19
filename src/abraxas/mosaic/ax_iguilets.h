// ax_iguilets.h ------------------------------------------------------------------------------------------------------------------
#pragma once

#include <ImFileBrowser.h>

//---------------------------------------------------------------------------------------------------------------------------------
 
class Ax_IGFilebrowzer : public ImGui::FileBrowser 
{      
    typedef bool    Loader( const std::string &fName);

    bool                    m_ActiveFlg;
    std::vector<const char*>    m_FileFilters;
    std::function< Loader>  m_Loader;
public:
    Ax_IGFilebrowzer( const std::string &title) :
        m_ActiveFlg( false)
    {  
        SetTitle( title);
    }

    ~Ax_IGFilebrowzer( void)
    {} 

    void    Invoke( const std::vector<const char*>& fileFilters, const std::function< Loader> &l)
    { 
        if ( m_ActiveFlg)
            return;
        m_FileFilters = fileFilters;
        SetFileFilters( fileFilters);
        m_Loader = l;
        m_ActiveFlg = true;
        Open();
    }

    bool    IsActive( void) { return m_ActiveFlg; }

    bool    Render( void)
    { 
        if ( !m_ActiveFlg)
            return false;

        Display();
        if ( HasSelected() && m_Loader)
        {
            std::string     fName = GetSelected().string();
            bool    res = m_Loader( fName);
            ClearSelected();
            if ( !res)
                return true;
        
            m_ActiveFlg = false;   
            m_Loader = std::function< Loader>();
            m_FileFilters.clear();     
            return true;    
        }
        return true;
    }
};

//---------------------------------------------------------------------------------------------------------------------------------
