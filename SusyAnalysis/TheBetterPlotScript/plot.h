#ifndef PLOT_H
#define PLOT_H

#include "PlotTools.h"

void GetPlotTools(PlotTools*& plotTools, std::string filename, std::string GeneratorFile="", unsigned factor=0);
void AddEvents(PlotTools*& plotTools, std::string filename, std::string GeneratorFile="");




#endif
