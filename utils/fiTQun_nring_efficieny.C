#include "TH2D.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TGaxis.h"
#include "TGraph.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TMath.h"
#include <iostream>

void fiTQun_nring_efficiency(TString fs){

  // Get the file/ttree
  TFile *f = new TFile(fs.Data(), "READ");
  TTree *t = (TTree*)f->Get("h1");

  int nEntries = t->GetEntries();

  int nring;
  unsigned int ip[10];

  t->SetBranchAddress("nring", &nring);
  t->SetBranchAddress("ip", &ip);

  for (int i = 0; i < nEntries; ++i){
    t->GetEntry(i);

    std::cout << "\n************************************************************************" << std::endl;
    std::cout << "Entry: " << i << std::endl;
    std::cout << "nRings: " << nring << "   TOP IP: " << ip[0] << std::endl;
    for (int j = 0; j < nring ; ++j){
      std::cout << "-- ring: " << j << " PID: " << ip[j] << std::endl;
    }
  }
}

