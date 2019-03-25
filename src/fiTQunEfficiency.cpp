// We use this exe to compare n number of TTree's.
// We insert all the input files with matching TTree's together with an output
// .root file, and the executable SHOULD compare all the files against each
// other and produce .png comparison plots for each branch that's not matching
// between the files, and .root with all the plots regardless if matching or
// not.
#include "core/muenalysis.h"
#include "analysis/comparison.h"
#include "fitqun/fitqunEfficiency.h"

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

//  fitqunEfficiency *fitqun = new fitqunEfficiency(core);
//  fitqun->setBranches();

  fqeff *fitqun = new fqeff(core);
  fitqun->setTLegendTitle( "woFlag" );
  fitqun->setTLegendTitle( "wFlag" );

  // Do Numu disappearance 
  fitqun->setCuts(true, // T2K numu disappearance cut
      false,            // T2K nue appearance cut
      false);           // T2K nueCC1pi cut
  fitqun->setOutputName("output_numu");
//  fitqun->plotLlh();
  fitqun->plotWalls(30, 0, 2);
//  fitqun->plotWalls(30, 1, 2);

  // Do nue appearance
  fitqun->setCuts(false, true, false);
  fitqun->setOutputName("output_nue");
//  fitqun->plotLlh();
  fitqun->plotWalls(30,0, 1);
//  fitqun->plotWalls(30,1, 1);
//
  // Do nue appearance
  fitqun->setCuts(false, false, true);
  fitqun->setOutputName("output_nueCC1pi");
//  fitqun->plotLlh();
  fitqun->plotWalls(30,0, 1);
//  fitqun->plotWalls(30,1, 1);
  

  return 0; 
}
