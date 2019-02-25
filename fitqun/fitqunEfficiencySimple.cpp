// This is a "simple" fiTQun efficiency class. Basically a glorified plotting
// script... It is fairly simplistic, but it will be expanded and generalized
// later.
#include "fitqunEfficiencySimple.h"


// Boring default constructor
fqeffsimp::fqeffsimp(){
  nbins = 10;
  cuts = false;
  outputfilename = "outputfilename.root";
}

// Overload the constructor with core
fqeffsimp::fqeffsimp(muenalysis *coreIn){
  // Set core
  core = coreIn;

  // Sort out the file name
  outputfilename = core->getOutputName();
  std::size_t pos;
  pos = outputfilename.find(".root");
  outputfilename = outputfilename.substr(0, pos);

  // Get the number of files
  nfiles = core->getNFiles();

  // Initialize plots and chains
  plots = new TH1D*[nfiles];
  chains = new TChain*[nfiles];

  // Now REALLY initialize each chain and it's parser
  for(int i = 0; i < nfiles; ++i){
    chains[i] = new TChain("h1");
    chains[i]->Add((core->getFileName(i)).c_str());

    // Make a new parser and add to a vector (one parser per TChain)
    fqbrparser *parserdummy = new fqbrparser();
    parser.push_back(parserdummy);
  }

  // The rest of the constructor...
  nbins = 10;
  cuts = false;
}

// Sets core 
void fqeffsimp::setCore(muenalysis *coreIn){
  // Set the core
  core = coreIn;

  // Sort out the output filename
  outputfilename = core->getOutputName();
  std::size_t pos;
  pos = outputfilename.find(".root");
  outputfilename = outputfilename.substr(0, pos);

  // Get the number of files
  nfiles = core->getNFiles();

  // Initialize plots and chains
  plots = new TH1D*[nfiles];
  chains = new TChain*[nfiles];

  // Now REALLY initialize each chain and it's parser
  for(int i = 0; i < nfiles; ++i){
    chains[i] = new TChain("h1");
    chains[i]->Add((core->getFileName(i) + "/h1").c_str());
    std::cout << "Adding " << (core->getFileName(i) + "/h1").c_str() << std::endl;

    // Make a new parser and add to a vector (one parser per TChain)
    fqbrparser *parserdummy = new fqbrparser();
    parser.push_back(parserdummy);
  }
}

// The main plotting function. For now it's just a llh plotter, but we will
// expand ang generalise this!
void fqeffsimp::plotLlh(){

  // Some initial asthetics
  // Maybe move it to the config...
  gStyle->SetOptStat(0);
  gStyle->SetPalette(51,0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleStyle(0);
  gStyle->SetTitleX(0.1f);
  gStyle->SetTitleW(0.8f);

  // Set the plot type template
  TH1D* dummy_template = new TH1D("fqmrnll[0]", "fqmrnll[0] comparison", nbins, 0, 60000);

  // Setup plots and set branches through parser.
  for(int i = 0; i < nfiles; ++i){
    plots[i] = (TH1D*)dummy_template->Clone();

    // This operator actually sets the branches. 
    (*parser[i])(chains[i]);
  }

  // Some sanity checks, just in case
  for(int i = 1; i < nfiles; ++i){
//    if(core->getNEvents(i-1) != core->getNEvents(i)){
    if(chains[i-1]->GetEntries()!= chains[i]->GetEntries()){
      std::cout << "FLAG: number of entries don't math between: " << std::endl;
    }
  }

  // Start plotting "fun". 
  TCanvas *c = new TCanvas("c", "c", 1000, 800);
  c->SetFillColor(0);

  // We will fill all the files/plots separately.
  for( int file = 0; file < nfiles; ++file){
    std::cout << "File: " << file << " nEntries: " << chains[file]->GetEntries() << std::endl;

    // Now go into the file and fill the histograms
    for(int i = 0; i < chains[file]->GetEntries(); ++i){

      // Get entry for this file/event. The parser contains has all the yummy values
      chains[file]->GetEntry(i);
      if(i == 1)
        parser[file]->PrintEvent(i);

      // Are the T2K cuts met?
      if(parser[file]->isWithinT2KCut())
        plots[file]->Fill(parser[file]->E->fqmrnll[0]);
    }
  }





}

