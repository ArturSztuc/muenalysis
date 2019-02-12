#include "muenalysis.h"

// Default constructor
muenalysis::muenalysis(){
  nFiles = 0;
  outputname = "dummy.root";
}


// Sets the input file and reads all the branches.
void muenalysis::setBranches(std::string file){
  std::cout << "Reading branches from file: " << file << std::endl;

  fileNames.push_back(file);

  // Load the file
  TFile *inFile = new TFile(file.c_str());

  // Load and push the ttree into ttree array
  trees.push_back((TTree*)inFile->Get("h1"));

  // Adding the chain to the chain array
  chains.push_back(new TChain("h1",""));
  chains.back()->Add(file.c_str());

  // Increment the number of files to be analysed
  nFiles++;

  // Get list and number of branches
  branchList.push_back((TObjArray*)chains.back()->GetListOfBranches());
  nBranches.push_back(branchList.back()->GetEntries());

  // Init a local vector of names of branches
  std::vector<std::string> brNames;

  // Init a local vector of branch types (int)
  std::vector<int> brType;

  int nints = -1;
  int nfloats = -1;
  int ndoubles = -1;
  int nnull = -1;

  // Init a local ector of branch indices
  std::vector<int> branchIndex;

  // Push to the vector of events per ttree
  // We will use it later e.g. for branch reading/debuggin
  nEvents.push_back(((TBranch*)branchList.back()->At(0))->GetEntries());
  
  std::cout << "About to start iteration through branches for " << file << std::endl;
  // Iterate through branches and get all the item type names
  for(int i = 0; i < nBranches.back(); i++){
    TBranch *br = (TBranch*)branchList.back()->At(i);
    std::string brName = std::string(br->GetName());
    brNames.push_back(brName);
    
    // Get the branch data type
    TClass *tclass;
    EDataType edatatype;
    br->GetExpectedType(tclass, edatatype);

    std::cout << "Branch : " << brName.c_str() << " Type int:"  << " Type str1: " << edatatype <<  std::endl;
    std::cout << " -- nLeaf: " << br->GetNleaves() << " Entries: " << br->GetEntries() << 
      " GetMaxBaskets: " << br->GetMaxBaskets() << std::endl;
    brType.push_back(edatatype);

    // Push index of the branch-type combination
    switch(edatatype){
      case 5:
        nfloats++;
        branchIndex.push_back(nfloats);
        break;
      case 3:
        nints++;
        branchIndex.push_back(nints);
        break;
      case 11:
        ndoubles++;
        branchIndex.push_back(ndoubles);
        break;
      default:
        nnull++;
        branchIndex.push_back(nnull);
        break;
    }
  }
  nInts.push_back(nints);
  nFloats.push_back(nfloats);
  nDoubles.push_back(ndoubles);
  nNulls.push_back(nnull);

  // Push vector of this file's branches files into main branch name matrix
  branchNames.push_back(brNames);

  // Push vector of this file's branch types into main branch type matrix
  branchType.push_back(brType);

  // Will igenerate and initialise vectors for each parameter
  setVectors(nFiles-1);
}

// Sets the parameter containers per file. Generates
// vector of vectors: Index 1: branch, Index 2: event
// Pushes that newly created vector to a vector of files
void muenalysis::setVectors(int fileIndex){

  // Generate a local vector of vectors of ints.
  // Index 1: branch, Index 2: event
  std::vector< std::vector< int > > intBranches;
  std::cout << "File Index; " << fileIndex << std::endl;
  std::cout << "\nAbout to generate a vector. Number of ints; " << nInts[fileIndex] << " Nuber of floats; " << nFloats[fileIndex] << " Number of doubles; " << nDoubles[fileIndex] << std::endl;
  for(int j = 0; j < nInts[fileIndex]; ++j){
    std::vector<int> intVec;
    for(int k = 0; k < nEvents[fileIndex]; ++k){
      intVec.push_back(int(0));
    }
    intBranches.push_back(intVec);
  }
  std::cout << "Generated " << intBranches.size() << " int branches with " <<
    intBranches[0].size() << " events each for file number " << fileIndex <<
    std::endl;
  // Load the local vector into global vctor of files
  i_paramValuesVector.push_back(intBranches); 

  // Generate a local vector of vectors of floats.
  // Index 1: branch, Index 2: event
  std::vector< std::vector< float > > floatBranches;
  for(int j = 0; j < nFloats[fileIndex]; ++j){
    std::vector<float> floatVec;
    for(int k = 0; k < nEvents[fileIndex]; ++k){
      floatVec.push_back(0.0);
    }
    floatBranches.push_back(floatVec);
  }
  std::cout << "Generated " << floatBranches.size() << " float branches with " <<
    floatBranches[0].size() << " events each for file number " << fileIndex <<
    std::endl;
  // Load the local vector into global vctor of files
  f_paramValuesVector.push_back(floatBranches); 

  // Generate a local vector of vectors of doubles.
  // Index 1: branch, Index 2: event
  std::vector< std::vector< double > > doubleBranches;
  for(int j = 0; j < nDoubles[fileIndex]; ++j){
    std::vector<double> doubleVec;
    for(int k = 0; k < nEvents[fileIndex]; ++k){
      doubleVec.push_back(0.0);
    }
    doubleBranches.push_back(doubleVec);
  }
  std::cout << "Generated " << doubleBranches.size() << " double branches with " <<
    doubleBranches[0].size() << " events each for file number " << fileIndex <<
    std::endl;
  // Load the local vector into global vctor of files
  d_paramValuesVector.push_back(doubleBranches); 
}


