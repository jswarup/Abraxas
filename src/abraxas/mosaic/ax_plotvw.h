// ax_plotvw.h  -------------------------------------------------------------------------------------------------------------------------
#pragma once

#include    "cyto/silo/cy_treeifc.h"

//---------------------------------------------------------------------------------------------------------------------------------

class Ax_PlotVw : public Cy_TreeIfc
{   
    NodePtr                 m_Layout;
    NodePtr                 m_Cntl;
    NodePtr                 m_Store; 
    Cy_FArr< NodePtr>       m_Species;
    
    Cy_FArr< uint8_t>                   m_SpeciesFlgs;
    Cy_FArr< Cy_Bunch< float>>          m_SpeciesVals;
    Cy_Bunch< float>                    m_Times;

    double                              m_TimeLow;
    double                              m_TimeHigh;
    bool                                m_PlayBeginFlg;
    ImPlotRect                          m_Limits;
    std::mutex                          m_Mutex;

public:
    Ax_PlotVw( void) :
        m_Layout( NULL), m_PlayBeginFlg( true)
    {}

    ~Ax_PlotVw( void)
    {
        Clear();
    }

    void    DoInit( NodePtr layout) 
    {
        Clear();
        m_Layout  = layout;
        m_Cntl = layout->NodeAt( 0);
        m_Store = layout->NodeAt( 2);
    }

    void    Clear( void)
    { 
        m_Species.Clear();
        m_SpeciesFlgs.Clear();
        m_SpeciesVals.Clear();
    }

    void    DoSetup( double time) 
    {  
        auto    speciesIfc = m_Store->NodeAt( 1);
        m_Species.DoInit( speciesIfc->SzNode(), NULL);
        m_SpeciesVals.DoInit( speciesIfc->SzNode());

        m_SpeciesFlgs.DoInit( speciesIfc->SzNode(), 0);
        Cy_USeg( 0, m_Species.Size()).Traverse( [&]( uint32_t k) {
            m_Species.SetAt( k, speciesIfc->NodeAt( k));
        });
        m_TimeLow = time;
        m_TimeHigh =  m_Cntl->AttrAt( 3).Value< double>();
    } 

    bool    MonitorSimulation(  uint32_t entInd, double time, uint32_t speciesInd, double speciesValue,  uint32_t clusterInd, uint32_t sz);
    
    auto    FetchMonitor( NodePtr layout) 
    { 
        DoInit( layout);
        return [&]( uint16_t succ, Cy_ChoreQueue *queue, uint32_t entInd, double time, uint32_t speciesInd, double speciesValue,  uint32_t clusterInd, uint32_t sz ) {
            if ( !m_Species.Size())
                DoSetup( time);
            return MonitorSimulation( entInd, time, speciesInd, speciesValue, clusterInd, sz);
        };
    } 

    bool    Render1( void);
    bool    Render( void);
};

//---------------------------------------------------------------------------------------------------------------------------------
