#include "comparison.h"

// Default constructor
comparison::comparison(){
  nFiles = 0;
  wrong = false;
  outputname = "dummy.root";
}

// Get the basics from the core
void comparison::setCore(muenalysis *coreIn){
  core = coreIn;
  outputname = core->getOutputName();
  nFiles = core->getNFiles();
  nBranches = core->getNBranches();
  nEvents = core->getNEvents();
}

void comparison::makeComparisons(){
  // We'll do a few checks first...
  // Need more than one file with TTree's to compare
  if(nFiles < 2){
    std::cerr << "ERROR: Only one .root file loaded!" << std::endl;
    std::exit(EXIT_FAILURE);
  }
  // Need to make sure there's an equal number of branches in each
  // file. Perhaps should do string comparison here...
  for(int i = 1; i < nFiles; ++i){
    if(nBranches[i] != nBranches[i-1]){
      std::cerr << "ERROR: Unequal number of branches in each file!" << std::endl;
      std::exit(EXIT_FAILURE);
    }

    // Compre number of events per file. Normally should be the same,
    // but it's probably fine if they differ; we're comparing histograms,
    // not event-by-event.
    if(nEvents[i] != nEvents[i-1]){
      std::cerr << "WARNING: unequal number of events per file." << std::endl;
      std::cerr << "-------: Probably should be fine, we're comparing histograms not events!" << std::endl;
    }
  }

  std::size_t pos;
  pos = outputname.find(".root");
  std::string outName = outputname.substr(0,pos);


  // Now we can start making TH1D's for each branch!
  std::cout << "Making comparisons of " << nFiles << "TTree's with " 
    << nBranches[0] << "branches each." << std::endl;


  // We need a lot of plots for this...
  TFile *fout = new TFile(outputname.c_str(), "recreate");
  plots = new TH1D*[nBranches[0]*nFiles];
  for(int i = 0; i < nBranches[0]; ++i){
    TCanvas * c1 = new TCanvas("c1", "c1", 800, 800);
    for(int j = 0; j < nFiles; ++j){
      int k = i*nFiles + j;

      // Generate plot for the branch
      core->getTTree(j)->Draw(core->getBranchName(j,i).c_str());
      plots[k] = (TH1D*)gPad->GetPrimitive("htemp");
      plots[k]->ResetBit(kCanDelete);
      if(j == 0) plots[k]->SetLineColor(1);
      if(j != 0) plots[k]->SetLineColor(1+j);

      if(j == 0)plots[k]->Write((core->getBranchName(0,i)+"_0").c_str());
      if(j != 0)plots[k]->Write((core->getBranchName(0,i)+"_1").c_str());

    }
    compareFiles(i);

    if(wrong){
      TCanvas * c2 = new TCanvas("c2", "c2", 800, 800);
      gStyle->SetOptStat(0);
      for(int j = 0; j < nFiles; ++j){
        int k = i*nFiles + j;
        if(j == 0) plots[k]->Draw();
        if(j != 0) plots[k]->Draw("SAME");
      }

      c2->SaveAs((outName+"_"+core->getBranchName(0,i)+"_comparison.png").c_str());
    }
    c1->Clear();
  }

  delete fout;
}

// This will check if specific branches in each file match.
// If they deviate by ~1% (change this later?) change the "wrong"
// flag to "true"
void comparison::compareFiles(int branch){
  // Reset the flag to "false"
  wrong = false;

  // bin heights
  double *binh;
  binh = new double[nFiles]();

  // Number of bins p/plot
  int nBins = plots[branch*nFiles]->GetNbinsX();

  // Iterate through each bin and compare this bin between files
  for(int j = 0; j < nBins; ++j){
    // If previous bin not matching, just forget about the rest and quit
    if(wrong) continue;
    
    // Save the bin contents between the files
    for(int i = 0; i < nFiles; ++i){
      int k = branch*nFiles + i;
      binh[i] = plots[k]->GetBinContent(j);
    }

    // Now we will compare all the bins against each other.
    // Compare current against prebious file (skip first)
    for(int i = 1; i < nFiles; ++i){
      // If previous bin not matching then forget about everything else and quit
      if(wrong) continue;
      if (binh[i-1] != binh[i] ){
        wrong = true;
        std::cout << "\n******************************************************************" << std::endl;
        std::cout << "Watch out, bins don't match, we will output the plot for this one!" << std::endl;
        std::cout << "Parameter: " << core->getBranchName(0,branch) << " Bin number: " << j << std::endl;
        std::cout << "Bin ratio: " << binh[i-1]/binh[i] << std::endl;
        std::cout << "Bin 1: " << binh[i-1] << " Bin 2: " << binh[i] << std::endl;
        std::cout << "******************************************************************\n" << std::endl;
      }
    }
  }
}

      


  

  





