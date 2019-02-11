// We use this exe to compare n number of TTree's.
// We insert all the input files with matching TTree's together with an output
// .root file, and the executable SHOULD compare all the files against each
// other and produce .png comparison plots for each branch that's not matching
// between the files, and .root with all the plots regardless if matching or
// not.
#include "core/muenalysis.h"
#include "analysis/comparison.h"
#include "analysis/cuts.h"

int main(int argc, char *argv[]) {

  // If less than 4 then we're missing either an output file or one of the
  // (minimum of 2!) input root files.
  if (argc < 4) {
    std::cerr << "./plotTracks inputFile.root inputFile2.root outputFile.root" << std::endl;
    exit(-1);
  }
  
  // Make a core object contain file/branch information
  muenalysis *core = new muenalysis();

  // Add new files to the core object
  for(int i = 1; i < argc-1; ++i)
    core->setBranches(argv[i]);

  // Set the output name
  core->setOutputName(argv[argc-1]);

  // Make a new comparison object
//  comparison *compare = new comparison();
//
//  // Link it with the core
//  compare->setCore(core);
//
//  // Make comparison plots
//  compare->makeComparisons();

  cuts *cutsmanager = new cuts();

  cutsmanager->addCut("nring", 1, "==");
//  cutsmanager->addCut("fqmrnring[0]", 1, "==");

  for(int i=0; i < cutsmanager->getNCuts();  ++i)
    std::cout << cutsmanager->getCutString(i) << std::endl;

  cutsmanager->setCore(core);
  cutsmanager->setCuts(0);

  for(int i =0; i < 900 ; ++i){
    cutsmanager->isCut(0, i);
  }

  return 0; 
}
