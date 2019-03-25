// fiTQun Branch Parser is capable of setting all the fiTQun branches for a
// given TChain. We use struct fqevent that contains all the data variables
// (with their correct types), and the "operator" member function (declaration
// and definition both in the header) can be used to overload/overwrite the
// SetBranchAddress for any TChain. Perhaps this is also possible to do for
// TTree?
// We can also use this class to set up some specific cuts.

#define __FQBRPARSER_CXX
#include "fitqunBranchParser.h"

// Default constructor
fqbrparser::fqbrparser(){
  E = new fqevent();
  reset_bools();
}

void fqbrparser::reset_bools(){
  is_wall = false;
  is_fqwall = false;
  is_fqtowall = false;
  is_fqfromwall = false;
  is_fqmrnll = false;
}


// Prints all the branches for the given event/entry. Might be useful for
// debugging, sanity checks etc. 
void fqbrparser::PrintEvent(int i){
  std::cout << "\n###########################################" << std::endl;
  std::cout << "# Example branch values, file " << i << ":" << std::endl;
  std::cout << "###########################################" << std::endl;

  std::cout << "# SK PARAMETERS:    " << std::endl;
  std::cout << "# -- wall:              "<<  E->wall << std::endl;

  std::cout << "# - SUBEVENT PARAMETERS: " << std::endl;
  std::cout << "# -- fqnse:             "<<  E->fqnse << std::endl;

  std::cout << "# - 1 RING PARAMETERS: " << std::endl;
  std::cout << "# -- fq1rmom[0][1]:     "<<  E->fq1rmom[0][1] << std::endl;
  std::cout << "# -- fq1rnll[0][1]:     "<<  E->fq1rnll[0][1] << std::endl;
  std::cout << "# -- fq1rpos[0][1][0]:  " << E->fq1rpos[0][1][0] << std::endl;
  std::cout << "# -- fq1rdir[0][1][0]:  " << E->fq1rdir[0][1][0] << std::endl;

  std::cout << "# - MULTI RING PARAMETERS: " << std::endl;
  std::cout << "# -- fqnmrfit:          "<<  E->fqnmrfit << std::endl;
  std::cout << "# -- fqmrpid[0][1]:     "<<  E->fqmrpid[0][1] << std::endl; // Might be wrong?
  std::cout << "# -- fqmrnring[0]:      "<<  E->fqmrnring[0] << std::endl;
  std::cout << "# -- fqmrnll[0]:        "<<  E->fqmrnll[0] << std::endl;

  std::cout << "# - USER DEFINED PARAMETERS: " << std::endl;
  std::cout << "# -- fqwall[0],[1],[2]:   "<<  E->fqwall[0] << " " << E->fqwall[1] << " " << E->fqwall[2] << std::endl;
  std::cout << "# -- fqtowall[0],[1],[2]: "<<  E->fqtowall[0] << " " << E->fqtowall[1] << " " << E->fqtowall[2] << std::endl;
  std::cout << "# -- fqfromwall[0],[1],[2]: "<<  E->fqfromwall[0] << " " << E->fqfromwall[1] << " " << E->fqfromwall[2] << std::endl;

  std::cout << "# -- skwall[0],[1],[2]:   "<<  E->skwall[0] << " " << E->skwall[1] << " " << E->skwall[2] << std::endl;
  std::cout << "# -- sktowall[0],[1],[2]: "<<  E->sktowall[0] << " " << E->sktowall[1] << " " << E->sktowall[2] << std::endl;

  std::cout << "###########################################" << std::endl;
  std::cout << std::endl;
}

void fqbrparser::Fill(){
  if(is_wall) th1d_wall->Fill(E->wall);
  if(is_fqwall) th1d_fqwall->Fill(E->fqwall[1]);
  if(is_fqtowall) th1d_fqtowall->Fill(E->fqtowall[1]);
  if(is_fqfromwall) th1d_fqfromwall->Fill(E->fqfromwall[1]);

  if(is_fqmrnll) th1d_fqmrnll->Fill(E->fqmrnll[0]);
}

void fqbrparser::fillVector(){
  if(is_wall) th1d_vector.push_back( th1d_wall );
  if(is_fqwall) th1d_vector.push_back( th1d_fqwall );
  if(is_fqtowall) th1d_vector.push_back( th1d_fqtowall );
  if(is_fqfromwall) th1d_vector.push_back( th1d_fqfromwall );

  if(is_fqmrnll) th1d_vector.push_back( th1d_fqmrnll );
}


void fqbrparser::InitPlots(){
  th1d_wall     = new TH1D("wall", "wall comparison", 30, 0, 2000);
  th1d_wall->GetXaxis()->SetTitle("wall");

  th1d_fqwall   = new TH1D("fqwall", "fqwall comparison", 30, 0, 2000);
  th1d_fqwall->GetXaxis()->SetTitle("fqwall for e hypothesis");

  th1d_fqtowall = new TH1D("fqtowall", "fqtowall comparison", 30, 0, 2000);
  th1d_fqtowall->GetXaxis()->SetTitle("fqtowall for e hypothesis");

  th1d_fqfromwall = new TH1D("fqfromwall", "fqfromwall comparison", 30, 0, 2000);
  th1d_fqfromwall->GetXaxis()->SetTitle("fqfromwall for e hypothesis");

  th1d_fqmrnll = new TH1D("fqmrnll[0]", "fqmrnll[0] comparison", 10, 0, 60000);
}

// Get's the custom entries.
void fqbrparser::GetEntry(){
  // Get fqtowall, fqwall, fqwall
  for(int i = 0; i < 7; ++i){
    // Okajima's wall
    E->sktowall[i]   = skutil::towall(E->fq1rpos[0][i], E->fq1rdir[0][1]);
    E->skwall[i] = skutil::wallsk(E->fq1rpos[0][i]);

    // FiTQun's wall
    E->fqfromwall[i]   = skutil::fromwall(E->fq1rpos[0][i], E->fq1rdir[0][1]);
    E->fqtowall[i]   = fqutil::fqtowall(E->fq1rpos[0][i], E->fq1rdir[0][1]);
    E->fqwall[i] = fqutil::fqwall(E->fq1rpos[0][i]);
  }
}
      


// Is the current Event within T2K Numu disappearance?
bool fqbrparser::isT2KnumuDis(){

  // Now get the cut
  bool T2KCUT = false;
  if(E->fqwall[2] > 50)
    if(E->fqtowall[2] > 250.0)
      if(E->fq1rmom[0][1] > 30)
        if(E->fqmrnring[0] == 1)
          if(E->fq1rnll[0][2]-E->fq1rnll[0][1] < 0.2*E->fq1rmom[0][1])
            if(E->fq1rmom[0][2] > 200)
              if(E->fqnse <= 2)
                T2KCUT = true;
  return T2KCUT;
}

// Is the current Event within T2K Nue appearance cut?
bool fqbrparser::isT2KnueApp(){

  // Now get the cut
  bool T2KCUT = false;
  if(E->fqwall[1] > 80)
    if(E->fqtowall[1] > 170.0)
      if(E->fq1rmom[0][1] > 30)
        if(E->fqmrnring[0] == 1)
          if(E->fq1rnll[0][2]-E->fq1rnll[0][1] > 0.2*E->fq1rmom[0][1])
            if(E->fq1rmom[0][1] > 100)
              if(E->fqnse == 1)
                T2KCUT = true;
  return T2KCUT;
}

// Is the current Event within T2K NueCC1pi sample cut?
bool fqbrparser::isT2KnueCC1pi(){

  // Now get the cut
  bool T2KCUT = false;
  if(E->fqwall[1] > 50)
    if(E->fqtowall[1] > 270.0)
      if(E->fq1rmom[0][1] > 30)
        if(E->fqmrnring[0] == 1)
          if(E->fq1rnll[0][2]-E->fq1rnll[0][1] > 0.2*E->fq1rmom[0][1])
            if(E->fq1rmom[0][1] > 100)
              if(E->fqnse == 2)
                T2KCUT = true;
  return T2KCUT;
}



  


