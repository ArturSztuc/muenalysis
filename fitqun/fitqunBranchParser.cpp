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
}


// Prints all the branches for the given event/entry. Might be useful for
// debugging, sanity checks etc. 
void fqbrparser::PrintEvent(int i){
  std::cout << "Stuff for this event, file " << i << ":" << std::endl;
  std::cout << "-- wall:          "<<  E->wall << std::endl;
  std::cout << "-- fqnse:         "<<  E->fqnse << std::endl;
  std::cout << "-- fq1rmom[0][1]: "<<  E->fq1rmom[0][1] << std::endl;
  std::cout << "-- fq1rnll[0][1]: "<<  E->fq1rnll[0][1] << std::endl;
  std::cout << "-- fqnmrfit:      "<<  E->fqnmrfit << std::endl;
  std::cout << "-- fqmrpid[0][1]: "<<  E->fqmrpid[0][1] << std::endl; // Might be wrong?
  std::cout << "-- fqmrnring[0]:  "<<  E->fqmrnring[0] << std::endl;
  std::cout << "-- fqmrnll[0]:    "<<  E->fqmrnll[0] << std::endl;
  std::cout << std::endl;
}

// Is the current Event within T2K cut?
bool fqbrparser::isWithinT2KCut(){
  bool T2KCUT = false;
  if(E->wall > 50)
    if(E->fq1rmom[0][1] > 30)
      if(E->fqmrnring[0] == 1)
        if(E->fq1rnll[0][2]-E->fq1rnll[0][1] < 0.2*E->fq1rmom[0][1])
          if(E->fq1rmom[0][2] > 200)
            if(E->fqnse <= 2)
              T2KCUT = true;
  return T2KCUT;
}


