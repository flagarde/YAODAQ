#include "CLI.hpp"
#include "Channel.hpp"
#include "Event.hpp"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TSpectrum.h"
#include "TTree.h"
#include "TTreeReader.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

//#### You can use CLI if you want  https://cliutils.github.io/CLI11/book/ ######
// To run the code see the help doing "./TemplateAnalysis -h"

int main(int argc, char** argv)
{
  CLI::App    app{"Analysis"};
  std::string file{""};
  app.add_option("-f,--file", file, "Name of the file to process")->required()->check(CLI::ExistingFile);
  int NbrEvents{0};
  app.add_option("-e,--events", NbrEvents, "Number of event to process", 0)->check(CLI::PositiveNumber);
  std::string nameTree{"Tree"};
  app.add_option("-t,--tree", nameTree, "Name of the TTree", "Tree");
  try
  {
    app.parse(argc, argv);
  }
  catch(const CLI::ParseError& e)
  {
    return app.exit(e);
  }
  //Open The file
  TFile fileIn(file.c_str());
  if(fileIn.IsZombie())
  {
    std::cout << "File Not Opened" << std::endl;
    std::exit(-3);
  }
  TTree* Run = static_cast<TTree*>(fileIn.Get(nameTree.c_str()));
  if(Run == nullptr || Run->IsZombie())
  {
    std::cout << "Problem Opening TTree \"Tree\" !!!" << std::endl;
    std::exit(-4);
  }
  Long64_t NEntries = Run->GetEntries();
  Event* event{nullptr};
  if(Run->SetBranchAddress("Events", &event))
  {
    std::cout << "Error while SetBranchAddress !!!" << std::endl;
    std::exit(-5);
  }
  for(Long64_t evt = 0; evt < NbrEvents; ++evt)
  {
    //Loop on events see event.hpp
    event->clear();
    Run->GetEntry(evt);
    for(unsigned int ch = 0; ch != event->Channels.size(); ++ch)
    {
      //Loop on channels see channel.hpp
    }
  }
  if(event != nullptr) delete event;
  if(Run != nullptr) delete Run;
  if(fileIn.IsOpen()) fileIn.Close();
  std::cout << "BYE !!!" << std::endl;
}
