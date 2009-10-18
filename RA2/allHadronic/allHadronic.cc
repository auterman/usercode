#include "handleHists.h"
#include "ConfigFile.h"

#include <string>
#include <iostream>

using namespace std;



int main(int argc, char *argv[])
{
  string configfilename = "config/default.cfg";
  if (argc==2)
    configfilename = argv[1];
  
  ConfigFile config(configfilename);
  
  handleHists * handle = new handleHists( &config );
  handle->PlotHistograms("resultplots.ps");
  handle->Print();

  //Here significances / limits could be calculated
  //...

  delete handle;
}
