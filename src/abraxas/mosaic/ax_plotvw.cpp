// ax_plotvw.cpp  -------------------------------------------------------------------------------------------------------------------------

#include  "utils/ax_includes.h"
#include  "abraxas/mosaic/ax_plotvw.h" 
#include  "implot.h"

 
//---------------------------------------------------------------------------------------------------------------------------------

bool    Ax_PlotVw::Render1( void)
{  
    static float xs1[1001], ys1[1001];
    for (int i = 0; i < 1001; ++i) {
        xs1[i] = i * 0.001f;
        ys1[i] = 0.5f + 0.5f * sinf(50 * (xs1[i] + (float)ImGui::GetTime() / 10));
    }
    static double xs2[20], ys2[20];
    for (int i = 0; i < 20; ++i) {
        xs2[i] = i * 1/19.0f;
        ys2[i] = xs2[i] * xs2[i];
    }
    auto my_lambda = [](int idx, void*) {
        double t = idx / 999.0;
        return ImPlotPoint(t, 0.5+0.5*std::sin(2*3.1416*10*t));
    };
 
    ImGui::Begin( "Line Graph", NULL, ImGuiWindowFlags_NoCollapse);
    if (ImPlot::BeginPlot("Line Plots")) {
        ImPlot::SetupAxes("x","y"); 
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle); 
        ImPlot::SetNextLineStyle(ImVec4(1,0.5f,1,1));
        ImPlot::PlotLineG("line", my_lambda, nullptr, 1000);
        ImPlot::EndPlot();
    } 
    ImGui::End();

    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------

bool    Ax_PlotVw::MonitorSimulation( uint32_t entInd, double time, uint32_t speciesInd, double speciesValue,  uint32_t clusterInd, uint32_t sz ) 
{ 
    m_SpeciesFlgs.SetAt( speciesInd, 1);
    if (( m_PlayBeginFlg = !sz) && (( entInd % 256) == 1))
    {
        std::lock_guard<std::mutex> lock( m_Mutex);
        Cy_USeg( 0, m_Species.Size()).Traverse( [&]( uint32_t k) {
            if ( !m_SpeciesFlgs.At( k))
                return;
            double  curConc = m_Species.At( k)->AttrAt( 1).Value< double>();
            m_SpeciesVals.PtrAt( k)->PushBack( float( curConc));
        });
        float   curTime = ( float) m_Store->AttrAt( 0).Value< double>();
        m_Times.PushBack( curTime);
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------

bool    Ax_PlotVw::Render( void)
{  
    if ( ! m_Times.Size())
        return true;
    std::lock_guard<std::mutex> lock( m_Mutex);
    ImGui::Begin( "Species Graph", NULL, ImGuiWindowFlags_NoCollapse); 
    const char  *xLabel = "Time";
    const char  *yLabel = "Conc";
    if (ImPlot::BeginPlot("##Plot", xLabel, yLabel, ImVec2(-1, 0), ImPlotFlags_None)) 
    {
        ImPlot::SetupAxis(ImAxis_X1, xLabel, ImPlotAxisFlags_AutoFit);
        ImPlot::SetupAxis(ImAxis_Y1, yLabel, ImPlotAxisFlags_AutoFit);

        m_Limits = ImPlot::GetPlotLimits(); 
        ImVec4  color(1,0.75f,0,1);
        ImPlot::SetNextLineStyle( color);
        Cy_USeg( 0, m_Species.Size()).Traverse( [&]( uint32_t k) {
            if ( !m_SpeciesFlgs.At( k))
                return;
            auto    *species = m_SpeciesVals.PtrAt( k);
            uint32_t speciesSz = species->Size();
            if ( !speciesSz)
                return;
            uint32_t timeSz = m_Times.Size();
            ImPlot::PlotLine("##item", m_Times.PtrAt( 0) -speciesSz +timeSz, species->PtrAt( 0), species->Size());
        }); 
        ImPlot::EndPlot();
    }
    ImGui::End();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
