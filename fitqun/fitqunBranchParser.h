#ifndef _fqbrparser_h_
#define _fqbrparser_h_

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <string>
#include <sstream>

#include "fqevent.h"
#include "analysis/skutil.h"

class fqbrparser{
  public:
    // Default constructor
    fqbrparser();

    // Operator, it sets fiTQun branches for a given TChain!
    // It's AWESOME
    void operator()( TChain *fchain );
    
    // Fill custom variables
    void GetEntry();

    // Prints an event. The integer can be anything you want, it prints it too...
    void PrintEvent(int i);

    // Checks if the current combination is within the T2K official cut.
    bool isT2KnumuDis();
    bool isT2KnueApp();
    bool isT2KnueCC1pi();

    // I would prefer it this was private, but we really need to access the variables!
    fqevent *E;


    // What do we actually want to plot?
    // FLAG : MAYBE SHOVE ALL THIS PLOTTING STUFF IN A SEPARATE CLASS?
    void reset_bools();
    bool is_wall;
    bool is_fqwall;
    bool is_fqtowall;
    bool is_fqfromwall;
    bool is_fqmrnll;
    void Fill();
    void fillVector();

    TH1D *th1d_wall;
    TH1D *th1d_fqwall;
    TH1D *th1d_fqtowall;
    TH1D *th1d_fqfromwall;

    TH1D *th1d_fqmrnll;
    std::vector< TH1D * > th1d_vector;
    void InitPlots();





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
  fchain->SetBranchAddress("fq1rpos", &E->fq1rpos);
  fchain->SetBranchAddress("fq1rdir", &E->fq1rdir);

  // fiTQun MR variables
  fchain->SetBranchAddress("fqnmrfit", &E->fqnmrfit);
  fchain->SetBranchAddress("fqmrpid", &E->fqmrpid);
  fchain->SetBranchAddress("fqmrnring", E->fqmrnring);
  fchain->SetBranchAddress("fqmrnll", &E->fqmrnll);
}
#endif
