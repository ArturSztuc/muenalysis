#ifndef _fqbrparser_h_
#define _fqbrparser_h_

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <string>
#include <sstream>

#include "fqevent.h"


class fqbrparser{
  public:
    // Default constructor
    fqbrparser();

    // Operator, it sets fiTQun branches for a given TChain!
    // It's AWESOME
    void operator()( TChain *fchain );

    // Prints an event. The integer can be anything you want, it prints it too...
    void PrintEvent(int i);

    // Checks if the current combination is within the T2K official cut.
    bool isWithinT2KCut();

    // I would prefer it this was private, but we really need to access the variables!
    fqevent *E;
};

#endif

#ifdef __FQBRPARSER_CXX
// Here we have the definition for our operator. It overloads the TChain branch setup!
void fqbrparser::operator()( TChain *fchain ){
  std::cout << "From within operator()!! " << std::endl;

  // SK variables
  fchain->SetBranchAddress("wall", &E->wall);

  // Some fiTQun variables...
  fchain->SetBranchAddress("fqnse", &E->fqnse);

  // fiTQun 1R variables
  fchain->SetBranchAddress("fq1rmom", &E->fq1rmom);
  fchain->SetBranchAddress("fq1rnll", &E->fq1rnll);

  // fiTQun MR variables
  fchain->SetBranchAddress("fqnmrfit", &E->fqnmrfit);
  fchain->SetBranchAddress("fqmrpid", &E->fqmrpid);
  fchain->SetBranchAddress("fqmrnring", E->fqmrnring);
  fchain->SetBranchAddress("fqmrnll", &E->fqmrnll);
}
#endif
