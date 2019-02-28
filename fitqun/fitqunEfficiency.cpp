// This is a fiTQun efficiency class. Basically a glorified plotting script...
// It is fairly simplistic, but it will be expanded and generalized later.
#include "fitqunEfficiency.h"


// Boring default constructor
fqeff::fqeff(){
  outputfilename = "outputfilename";
  nbins = 10;

  T2KCUTnumuDis = false;
  T2KCUTnueApp = false;
  T2KCUTnueCC1pi = false;
}

// Overload the constructor with core
fqeff::fqeff(muenalysis *coreIn){
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
  T2KCUTnumuDis = false;
  T2KCUTnueApp = false;
  T2KCUTnueCC1pi = false;
}

// Sets core 
void fqeff::setCore(muenalysis *coreIn){
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

// Sets the cuts.
void fqeff::setCuts(bool numu, bool nue, bool nuecc1pi){
  T2KCUTnumuDis = numu;
  T2KCUTnueApp = nue;
  T2KCUTnueCC1pi = nuecc1pi;

  if(numu)
    std::cout << "FQEFF: Running numu disappearance cuts!" << std::endl;
  if(nue)
    std::cout << "FQEFF: Running nue appearance cuts!" << std::endl;
  if(nuecc1pi)
    std::cout << "FQEFF: Running nueCC1pi sample cuts!" << std::endl;
}

// The main plotting function. For now it's just a llh plotter, but we will
// expand ang generalise this!
void fqeff::plotLlh(){

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
      parser[file]->GetEntry();

      if(i == 1)
        parser[file]->PrintEvent(file);

      // Are the T2K cuts met?
      if((T2KCUTnumuDis)&&(!parser[file]->isT2KnumuDis()))
        continue;
      if((T2KCUTnueApp)&&(!parser[file]->isT2KnueApp()))
        continue;
      if((T2KCUTnueCC1pi)&&(!parser[file]->isT2KnueCC1pi()))
        continue;

      plots[file]->Fill(parser[file]->E->fqmrnll[0]);

      // Prints first event for each file, just for dome extra debugging.
    }
    // Scale the histogram to get an "efficiency". In this case the denominator
    // is the pre-cut number of events
    plots[file]->Scale(1/(double)chains[file]->GetEntries());
    
    // Apply colors. Starting with Black
    plots[file]->SetLineColor(file+1);

    // The rest of the asthetics (local)
    plots[file]->GetXaxis()->SetTitle("Best fit fiTQun -lnL (fqmrnll[0])");
    plots[file]->GetYaxis()->SetTitle("Tagging efficiency");

    plots[file]->SetLineWidth(2);

    if (file==0)
      plots[file]->GetYaxis()->SetTitleOffset(1.5);

  }
  // The rest of the asthetics (global)
  gPad->SetLeftMargin(0.15);

  // Make a legend!
  TLegend *leg = new TLegend(0.48, 0.7, 0.89, 0.89);
  leg->SetFillColor(10);
  leg->SetLineColor(0);
  leg->SetTextSize(0.05);
  for(int i = 0; i < nfiles ; ++i){
    leg->AddEntry(plots[i], legendTitles[i].c_str());
    if(i==0)
      plots[i]->Draw();
    else
      plots[i]->Draw("SAME");
  }
  leg->Draw("SAME");
  c->SaveAs((outputfilename + "_eff_fqmrnll.png").c_str());
}


