#include "cuts.h"
#include <sstream>

cut::cut(){

  type = -1;
  bName1 = (char*)"";
  bName2 = (char*)"";
  intNumber = -1;
  floatNumber = -1;
  doubleNumber = -1;
  boolNumber = false;
  operatorStr = "";
}

cut::cut(char *sname1, char *op){
  type = -1;
  bName1 = sname1;
  bName2 = (char*)"";
  intNumber = -1;
  floatNumber = -1;
  doubleNumber = -1;
  boolNumber = false;
  operatorStr = op;
}

cut::~cut(){}

cuts::cuts(){
  nCuts = 0;
}


void cuts::addCut(char* sname1, char* sname2, char* op){
  cut newCut(sname1, op);
  newCut.bName2 = sname2;
  newCut.type = 0;
  cutsVector.push_back(newCut);
  nCuts += 1;
}

void cuts::addCut(char* sname1, int number, char* op){
  cut newCut(sname1, op);
  newCut.intNumber = number;
  newCut.type = 1;
  cutsVector.push_back(newCut);
  nCuts += 1;
}

void cuts::addCut(char* sname1, float number, char* op){
  cut newCut(sname1, op);
  newCut.floatNumber = number;
  newCut.type = 2;
  cutsVector.push_back(newCut);
  nCuts += 1;
}

void cuts::addCut(char* sname1, double number, char* op){
  cut newCut(sname1, op);
  newCut.doubleNumber = number;
  std::cout << "Double number: " << number << " from cut: " << newCut.doubleNumber << std::endl;
  newCut.type = 3;
  cutsVector.push_back(newCut);
  nCuts += 1;
}

void cuts::addCut(char* sname1, bool number, char* op){
  cut newCut(sname1, op);
  newCut.boolNumber = number;
  newCut.type = 4;
  cutsVector.push_back(newCut);
  nCuts += 1;
}


std::string cuts::getCutString(int n){

  int type = cutsVector[n].type;

  std::stringstream sstream;
  sstream << cutsVector[n].bName1 << " " << cutsVector[n].operatorStr << " " ;
  std::string cutString;

  switch(type)
  {
    case 0:
      {
        sstream << cutsVector[n].bName2;
        break;
      }
    case 1:
      {
        sstream << cutsVector[n].intNumber;
        break;
      }
    case 2:
      {
        sstream << cutsVector[n].floatNumber;
      }
    case 3:
      {
        sstream << cutsVector[n].doubleNumber;
        break;
      }
    case 4:
      {
        char boo[10];
        if(cutsVector[n].boolNumber == false)
          sprintf(boo,"false");
        else
          sprintf(boo,"true");
        sstream << boo;//cutsVector[n].boolNumber;
        break;
      }
    default:
      std::cerr << __FILE__ << "::" << __LINE__ << " type " << type << " not implemented!" << std::endl;
      return 0;
  }
  cutString = sstream.str();
  return cutString;
}

// Sets the core to access branches, files etc.
void cuts::setCore(muenalysis *coreIn){
  core = coreIn;
}


// Sets the branches based on our cuts
void cuts::setCuts(int file){
  for(int i = 0; i < nCuts; ++i){
    if(cutsVector[i].type == 1){
      core->getTTree(file)->SetBranchAddress(
          cutsVector[i].bName1,
          &cutsVector[i].iNum);
    }
    else if(cutsVector[i].type == 2){
      core->getTTree(file)->SetBranchAddress(
          cutsVector[i].bName1,
          &cutsVector[i].fNum);
    }
    else if(cutsVector[i].type == 3){
      core->getTTree(file)->SetBranchAddress(
          cutsVector[i].bName1,
          &cutsVector[i].dNum);
    }
    else if(cutsVector[i].type == 4){
      core->getTTree(file)->SetBranchAddress(
          cutsVector[i].bName1,
          &cutsVector[i].bNum);
    }
    else{
      std::cerr << __FILE__ << ":" << __LINE__ << " filetype: " << cutsVector[i].type << " not supported yet." << std::endl;
    }
  }
}


void cuts::isCut(int file, int entry){

  for(int i = 0; i < nCuts; ++i){
    core->getTTree(file)->GetBranch(cutsVector[i].bName1)->GetEntry(entry);
    std::cout << "Cut: " << getCutString(i).c_str() << " Entry: " << cutsVector[i].iNum << std::endl;
  }
  
}


