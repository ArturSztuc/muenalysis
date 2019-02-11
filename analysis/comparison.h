#ifndef _comparison_h_
#define _comparison_h_

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <sstream>
#include <math.h>
#include <string>

#include "TChain.h"
#include "TFile.h"

#include "TObjArray.h"
#include "TBranch.h"
#include "TLine.h"
#include "TString.h"
#include "TStyle.h"
#include "TStopwatch.h"

#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"

#include "TVectorD.h"
#include "TCanvas.h"
#include "TColor.h"

#include "core/muenalysis.h"

class comparison{

  public:
    // Default constructor
    comparison();

    // Sets branches for a given file
    void makeComparisons();
    void makePlots();
    void setCore(muenalysis *coreIn);

  private:

    muenalysis *core;

    std::string outputname;

    // int number of branches within TTree
    // Index: file with TTree
    std::vector<int> nBranches;

    // int number of events within TTree
    // Index: file with TTree
    std::vector<int> nEvents;

    // Index 1; file with TTree
    TH1D **plots;

    // Number of files to compare
    int nFiles;

    // Actually compares all the plots from different files
    // for a given branch
    void compareFiles(int branch);

    // Link branches to datatype
    std::vector< std::vector< int > > branchType;

    // boolean to see if specific branches match
    bool wrong;
};
#endif
