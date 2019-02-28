// This is a fiTQun efficiency class. Basically a glorified plotting
// script... It is fairly listic, but it will be expanded and generalized
// later.
#ifndef _fqeff_h_
#define _fqeff_h_

#include "TH2D.h"
#include "TH1D.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
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
#include <sstream>

#include "core/muenalysis.h"
#include "analysis/skutil.h"
#include "fitqunBranchParser.h"

class fqeff {
  public:
    // Constructors
    fqeff();
    fqeff(muenalysis *coreIn);

    // Set branches etc.
    void setCore(muenalysis *coreIn);

    // Nbins
    void setNBins(int nBinsIn){ nbins = nBinsIn; };

    // Set TLegend (it appends to a vector of strings)
    void setTLegendTitle(std::string legT){ legendTitles.push_back(legT); };

    // Sets cuts
//    void setCutsBool(bool cutsIn){ cuts = cutsIn; };

    // Sets output name
    void setOutPrefix(std::string prefix);
    void setOutPostfix(std::string postfix);
    void setOutputName(std::string outName){ outputfilename = outName; };

    void setCuts(bool numu, bool nue, bool nuecc1pi);

    // Make llh plots
    void plotLlh();

  private:
    // Core 
    muenalysis *core;

    // Store the TChains and plots locally
    TChain **chains;
    TH1D **plots;

    // Number of histogram bins
    int nbins;

    // Legend titles
    std::vector<std::string> legendTitles;
    std::vector< fqbrparser* > parser;

    // Do we apply T2K cuts?
    bool T2KCUTnumuDis;
    bool T2KCUTnueApp;
    bool T2KCUTnueCC1pi;

    // Number of files to nalyse;
    int nfiles;


    // Output filename
    std::string outputfilename;

};
#endif
