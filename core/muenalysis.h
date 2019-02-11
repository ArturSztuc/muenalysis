#ifndef _muenalysis_h_
#define _muenalysis_h_

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

class muenalysis{

  public:
    // Default constructor
    muenalysis();

    // Sets branches for a given file
    void setBranches(std::string filename);
    void setOutputName(std::string fname){ outputname = fname; };

    // Getters!
    std::string getOutputName(){ return outputname; };
    int getNFiles(){ return nFiles; };
    std::vector<int> getNBranches(){ return nBranches; };
    std::vector<int> getNEvents(){ return nEvents; };
    TTree *getTTree(int i){ return trees[i]; };
    TChain *getTChain(int i){ return chains[i]; };
    std::string getBranchName(int i, int j){ return branchNames[i][j]; };
    std::string getFileName(int i){ return fileNames[i]; };

  private:

    std::string outputname;

    // Index 1; file with Tree :: index 2; branches within TTree
    std::vector< std::vector< std::string > > branchNames;

    std::vector< std::string > fileNames;

    // TTree vector
    std::vector< TTree* > trees;

    // Index; file with TTree
    std::vector< TChain* > chains;

    // TObj list of branches
    // Index: file with TTree
    std::vector< TObjArray* > branchList;

    // int number of branches within TTree
    // Index: file with TTree
    std::vector<int> nBranches;

    // int number of events within TTree
    // Index: file with TTree
    std::vector<int> nEvents;

    // Number of files to compare
    int nFiles;

    // Sets the vector of vectors of
    // branches with parameters per file
    void setVectors(int fileIndex);

    // Link branches to datatype
    std::vector< std::vector< int > > branchType;

    // Number of branches of a specific datatype
    std::vector<int> nInts;
    std::vector<int> nFloats;
    std::vector<int> nDoubles;
    std::vector<int> nNulls;

    std::vector< std::vector < std::vector < int > > > i_paramValuesVector;
    std::vector< std::vector < std::vector < float > > > f_paramValuesVector;
    std::vector< std::vector < std::vector < double > > > d_paramValuesVector;

};

#endif
