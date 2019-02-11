#include "fitqunEfficiency.h"

// Default constructor definition
fitqunEfficiency::fitqunEfficiency(){
  treename = "fitqun_efficiency";
  treedescription = "Efficiency of fitqun fit";

  treeNum = 0;
}

// Constructor overloaded with core
fitqunEfficiency::fitqunEfficiency(muenalysis *coreIn){
  treename = "fitqun_efficiency";
  treedescription = "Efficiency of fitqun fit";

  treeNum = 0;

  setCore(coreIn);
}

fitqunEfficiency::fitqunEfficiency(muenalysis *coreIn, int treenumber){
  treename = "fitqun_efficiency";
  treedescription = "Efficiency of fitqun fit";

  treeNum = treenumber;

  setCore(coreIn);
}

// Sets core
void fitqunEfficiency::setCore(muenalysis *coreIn){
  core = coreIn;

  // Make default namebase
  outputname = core->getOutputName();
  std::size_t pos;
  pos = outputname.find(".root");
  outputname = outputname.substr(0,pos);

}

void fitqunEfficiency::setName(char* name){
//  outputname.clear();
  outputname = name;
}

void fitqunEfficiency::appendName(char* name){
  outputname += name;
}


// Will set the branches using core
void fitqunEfficiency::setBranches(){
  // no core, no branches to set
  if(!core){
    std::cerr << __FILE__ << ":" << __LINE__ << " Need to set core before setBranches!" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  // Set the output file/tree names
  outFile = new TFile((outputname+"_fiTQun_efficiencyies.root").c_str(), "recreate");
  outTree = new TTree(treename.c_str(), treedescription.c_str());

  TChain *chain = new TChain("h1");
  chain->Add(core->getFileName(treeNum).c_str());

  /******************************************/
  /*          SETTING BRANCHES              */
  /******************************************/
  /* This is not nice. Need to shift this   */
  /* to .h, preferebly into some compact    */
  /* struct!                                */
  /*----------------------------------------*/
  /*----------------------------------------*/
  /* And maybe do it from the core, that way*/
  /* We won't have to do it for each class  */
  /******************************************/

  // Set input branches (need to add momentum)
  chain->SetBranchAddress("nring", &br.nring);
  chain->SetBranchAddress("ip", &br.ip);
  chain->SetBranchAddress("fqmrnring", &br.fqmrnring);
  chain->SetBranchAddress("fqmrpid", &br.fqmrpid);

  // Set output branches (need to add momentum)
  outTree->Branch("is_nring", &out.is_nring, "is_ring/O");
  outTree->Branch("is_pid_all", &out.is_pid_all, "is_pid_all/O");
  outTree->Branch("is_pid_first", &out.is_pid_first, "is_pid_first/O");
  outTree->Branch("is_pid_all_nring", &out.is_pid_all_nring, "is_pid_all_nring/O");

  outTree->Branch("nring", &br.nring, "nring/I");
  outTree->Branch("ip", &br.ip, "ip[10]/b");
  outTree->Branch("fqmrnring", &br.fqmrnring, "fqmrnring[10]/I");
  outTree->Branch("fqmrpid", &br.fqmrpid, "fqmrpid[15][6]");

  // Get number of entries (Do it from core!)
  int nEntries = chain->GetEntries();

  // Iterate through all the events
  for (int i = 0; i < nEntries ; ++i){
    chain->GetEntry(i);

    if (br.nring == br.fqmrnring[0])
      out.is_nring = true;
    else
      out.is_nring = false;


    if (int(br.ip[0]) == br.fqmrpid[0][0])
      out.is_pid_first = true;
    else
      out.is_pid_first = false;

    for(UInt_t j = 0; j < br.nring; ++j){
      out.is_pid_all = true;
      if( int(br.ip[j]) != br.fqmrpid[0][j]+1 )
        out.is_pid_all = false;
    }

    out.is_pid_all_nring = true;
    if (br.nring != br.fqmrnring[0])
      out.is_pid_all_nring = false;
    else{
      for(UInt_t j = 0; j < br.nring; ++j){
        if( int(br.ip[j]) != br.fqmrpid[0][j]+1 )
          out.is_pid_all_nring = false;
      }
    }

    outTree->Fill();
  }
  outTree->Write(treename.c_str());
  outFile->Close();
}

