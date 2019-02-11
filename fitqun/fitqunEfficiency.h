#ifndef _FITQUNEFFICIENCY_H_
#define _FITQUNEFFICIENCY_H_
#include "core/muenalysis.h"
#include "TClassTable.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include "TClassTable.h"
#include "TSystem.h"
#include "TROOT.h"

// Struct containing information about each branch
  
//struct nRingsTuple{
//  std::string bName; 
//  int nrings; };
//
//struct pidTuple{
//  std::string bName; 
//  int pid; };
//
//struct nRingsFQ{
//  std::string bName; 
//  int nRings[10][10]; };
//
//struct pidFQ{
//  std::string bName; 
//  int pid[10][10]; };
//};

struct Event{
  // MC truth
  UInt_t nring;   // Number of rings
  UChar_t ip[10]; // PID

  // fiTQun values
  UInt_t fqmrnring[10];   // Number of rings
  UInt_t fqmrpid[15][6];  // PID

  Event(){}
};

struct OutEvent{
  bool is_nring;
  bool is_pid_all;
  bool is_pid_first;
  bool is_pid_all_nring;
}

class fitqunEfficiency
{

  public:
    // Default constructor
    fitqunEfficiency();

    // Constructor with setup core
    fitqunEfficiency(muenalysis *coreIn);

    fitqunEfficiency(muenalysis *coreIn, int treenumber);

    // Sets the core
    void setCore(muenalysis *coreIn);

    // Sets the required branches
    void setBranches();

    // Sets the output name
    void setName(char *name);
    void appendName(char *name);

    void operator()( TChain *fChain );



  private:
    // Core of the muenalysis
    muenalysis *core;

    // Output file and tree
    TFile *outFile;
    TTree *outTree;

    // Output filename
    std::string outputname;

    // Tree name and description
    std::string treename;
    std::string treedescription;

    // Which tree are we selecting?
    int treeNum;

    Event br;
    OutEvent out;

};


//void muenalysis::operator()( TChain *fChain ){
//  // Set the bloody IP's...
//  fChain->SetBranchAddress("nring", &br.nring);
//  fChain->SetBranchAddress("ip", &br.ip);
//  std::cout << "FLAG PARSING" << std::endl;
//}


#endif
