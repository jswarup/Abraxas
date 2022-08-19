// ax_plottest.cpp -----------------------------------------------------------------------------------------------------------------------

#include    "utils/ax_includes.h"

bool resizeLinePlot = false; 
std::vector< double > xData, yData;

//---------------------------------------------------------------------------------------------------------------------------------

static void LoadData(std::vector< double >* xData, std::vector< double >* yData)
{
    //srand(time(0));
    double randMax = 100;

    xData->resize(5000);
    yData->resize(5000);

    for (int i = 0; i < 5000; i++) {
        xData->at(i) = 86400 * i + 852163200;
        yData->at(i) = ((double)rand() / (double)(RAND_MAX)) * randMax;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------

void    ax_PlotTest( void)
{
    ImGui::Begin("Graphs");

    if (ImGui::Button("LoadData")) {
        LoadData(&xData, &yData);
        resizeLinePlot = true;
    }

    
    //ImPlot::LinkNextPlotLimits(&lineXMin, &lineXMax, NULL, NULL);

    // if (resizeLinePlot) { resizeLinePlot = false;  ImPlot::FitNextPlotAxes(); }

    if (ImPlot::BeginPlot("Line Graph##Line", "Day", "AUD", ImVec2(-1, 0), ImPlotFlags_NoBoxSelect, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit)) {

        ImPlot::PlotLine("Line Graph##Data", xData.data(), yData.data(), ( int)  yData.size());

        ImPlot::EndPlot();
    }



    //ImPlot::LinkNextPlotLimits(&lineXMin, &lineXMax, NULL, NULL);

    if (ImPlot::BeginPlot("Bar Graph##Line", "Day", NULL, ImVec2(-1, 0), ImPlotFlags_NoLegend | ImPlotFlags_NoBoxSelect, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit)) {

        for (int i = 0; i < xData.size(); i++) {
            if (i == 0) {
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0, 1, 0, 1));
                ImPlot::PushStyleColor(ImPlotCol_Fill, ImVec4(0, 1, 0, 1));
            }
            else
            {
                if (yData[i - 1] >= yData[i]) {   //Gone down
                    ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1, 0, 0, 1));
                    ImPlot::PushStyleColor(ImPlotCol_Fill, ImVec4(1, 0, 0, 1));
                }
                else //Gone up
                {
                    ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0, 1, 0, 1));
                    ImPlot::PushStyleColor(ImPlotCol_Fill, ImVec4(0, 1, 0, 1));
                }
            }

            ImPlot::PlotBars("Bar Graph##Line Individual", &xData[i], &yData[i], 1, 86400 * 0.25f);


            ImPlot::PopStyleColor();
            ImPlot::PopStyleColor();

        }

        ImPlot::EndPlot();
    }


    ImGui::End();

}

//---------------------------------------------------------------------------------------------------------------------------------
