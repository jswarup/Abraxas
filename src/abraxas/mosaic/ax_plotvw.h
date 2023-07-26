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
    

    std::mutex                          m_StoreMutex;
    std::mutex                          m_RenderMutex;
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

        void    CopyFrom( PlotData *plotData)
        {
            m_Limits = plotData->m_Limits;
            Cy_USeg( 0, m_SpeciesFlgs.Size()).Traverse( [&]( uint32_t k) {
                m_SpeciesFlgs.SetAt( k, plotData->m_SpeciesFlgs.At( k));
            });
            Cy_USeg( 0, m_AllSpeciesVals.Size()).Traverse( [&]( uint32_t i) {
                m_AllSpeciesVals.SetAt( i, plotData->m_AllSpeciesVals.At( i));
            });            
            m_Times = plotData->m_Times; 
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
        m_PlotStore = &m_PlotData1;
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
            if ( sz) 
                return true;
            bool    res = MonitorSimulation( entInd, time, speciesInd, speciesValue, clusterInd, sz);
            if ( !res)
                return false;
            {
                std::lock_guard<std::mutex>     lock( m_RenderMutex);
                std::swap( m_PlotRender, m_PlotStore);
            }
            if ( !queue)
                m_PlotStore->CopyFrom( m_PlotRender);
            else
                queue->EnqueueJob( queue->Construct( succ, [&]( uint16_t succId, Cy_ChoreQueue *queue){
                    m_PlotStore->CopyFrom( m_PlotRender);
                }));
            return true;
        };
    } 

    bool    Render1( void);
    bool    Render( void);
};

//---------------------------------------------------------------------------------------------------------------------------------
