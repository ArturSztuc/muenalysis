#include "TH2D.h"
#include "TH1D.h"
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
#include <string>


//void fiTQun_nring_comparison(TString fs1, TString fs2, TString l1="File 1", TString l2="File 2"){
void fiTQun_nring_comparison(std::string fs1, std::string fs2, TString l1="File 1", TString l2="File 2", std::string outputnameprefix = "h_fqmrnring_"){

  // Some initial asthetics
  // Maybe move it to the config...
  gStyle->SetOptStat(0);
  gStyle->SetPalette(51,0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleStyle(0);
  gStyle->SetTitleX(0.1f);
  gStyle->SetTitleW(0.8f);

  // Get the files/ttrees
  // Could move it to the library and use core
  TFile *f1 = new TFile(fs1.c_str(), "READ");
  TFile *f2 = new TFile(fs2.c_str(), "READ");

  TTree *t1 = (TTree*)f1->Get("h1");
  TTree *t2 = (TTree*)f2->Get("h1");

  // Set the histograms
  TH1D *nring_hist1 = new TH1D("fqmrnring[0]", "fqmrnring[0] comparison", 6, 0, 6);
  TH1D *nring_hist2 = new TH1D("fqmrnring[0]", "fqmrnring[0] comparison", 6, 0, 6);

  // Get number of entries
  int nEntries1 = t1->GetEntries();
  int nEntries2 = t2->GetEntries();

  if(nEntries1 != nEntries2)
    std::echo << "FLAG: number of entries don't match between the two files!" << std::endl;

  int fqnmrfit;
  int fqmrnring1[150];
  int fqmrnring2[150];
  int fqmrpid1[150][6];
  int fqmrpid2[150][6];

  t1->SetBranchAddress("fqnmrfit", &fqnmrfit);
  t1->SetBranchAddress("fqmrnring", &fqmrnring1);
  t1->SetBranchAddress("fqmrpid", &fqmrpid1);
  t2->SetBranchAddress("fqmrnring", &fqmrnring2);
  t2->SetBranchAddress("fqmrpid", &fqmrpid2);

  // Still try to compare branches if number of entries doesnt match...
  int nEntries = 0;
  if(nEntries1 > nEntries2)
    nEntries = nEntries2;
  else if (nEntries2 > nEntries1)
    nEntries = nEntries1;
  else
    nEntries = nEntries1;


  TCanvas *c = new TCanvas("c", "c", 1000, 800);
  c->SetFillColor(10);

  for (int i = 0; i < nEntries; ++i){
    std::cout << "integer?? " << i << std::endl;
    t1->GetEntry(i);
    t2->GetEntry(i);
 //   std::cout << "\n************************************************************************" << std::endl;
    std::cout << "Entry: " << i << std::endl;
//    std::cout << "fqmrnring1[0]: " << fqmrnring1[0] << " fqmrpid1[0][0]: " << fqmrpid1[0][0] << std::endl;

    nring_hist1->Fill(fqmrnring1[0]);
    nring_hist2->Fill(fqmrnring2[0]);
    //for (int j = 0; j < fqmrnring ; ++j){
    //  std::cout << "-- ring: " << j << " PID: " << fqmrpid[j] << std::endl;
    //}
  }

  // Asthetics
  nring_hist2->SetLineColor(kRed);
  nring_hist1->GetXaxis()->SetTitle("Best fit fitqun nring (fqmrnrings[0])");
  nring_hist1->GetYaxis()->SetTitle("Number of MC events");

  nring_hist1->SetLineWidth(2);
  nring_hist2->SetLineWidth(2);

  gPad->SetLeftMargin(0.15);
  nring_hist1->GetYaxis()->SetTitleOffset(1.5);

  TLegend *leg = new TLegend(0.48, 0.7, 0.89, 0.89);
  leg->SetFillColor(10);
  leg->SetLineColor(0);
  leg->SetTextSize(0.05);
  leg->AddEntry(nring_hist1, l1.Data());
  leg->AddEntry(nring_hist2, l2.Data());

  nring_hist1->Draw();
  nring_hist2->Draw("SAME");
  leg->Draw("SAME");
  c->SaveAs((outputnameprefix + ".png").c_str());
}

