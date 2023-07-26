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
    

    std::mutex                          m_Mutex;
    double                              m_TimeLow;
    double                              m_TimeHigh;
    struct PlotData
    { 
        ImPlotRect                          m_Limits;
        Cy_FArr< uint8_t>                   m_SpeciesFlgs;
        Cy_FArr< Cy_Bunch< float>>          m_AllSpeciesVals;
        Cy_Bunch< float>                    m_Times;

        void    Clear( void)
        {
            m_SpeciesFlgs.Clear();
            m_AllSpeciesVals.Clear();
        }
        void    DoSetup( uint32_t szNode)
        {
            m_AllSpeciesVals.DoInit( szNode); 
            m_SpeciesFlgs.DoInit( szNode, 0);
        }
    };
    
    PlotData                                m_PlotData0; 
    PlotData                                m_PlotData1; 
    PlotData                                *m_PlotRender;
    PlotData                                *m_PlotStore;
public:
    Ax_PlotVw( void) :
        m_Layout( NULL) 
    {
        m_PlotRender = &m_PlotData0;
        m_PlotStore = &m_PlotData0;
    }

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
        m_PlotData0.Clear();
        m_PlotData1.Clear();
    }

    void    DoSetup( double time) 
    {  
        auto        speciesIfc = m_Store->NodeAt( 1);
        uint32_t    szNode = speciesIfc->SzNode();
        m_Species.DoInit( szNode, NULL);
        m_PlotData0.DoSetup( szNode); 
        m_PlotData1.DoSetup( szNode); 
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
