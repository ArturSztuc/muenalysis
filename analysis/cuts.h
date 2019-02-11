#ifndef _cuts_h_
#define _cuts_h_

#include <iostream>
#include <vector>

#include "core/muenalysis.h"

// This is a sctuct that contains the cuts themselves.
// We will generate an array of these!
struct cut
{
  cut();
  cut(char *sname1, char *op);
  ~cut(); 


  // type = 0 ; branch vs branch
  // type = 1 ; branch vs int
  // type = 2 ; branch vs float
  // type = 3 ; branch vs double
  // type = 4 ; branch vs bool
  int type; 

  // Name of branches and numbers to compare
//  std::string bName1;
//  std::string bName2;
  char *bName1;
  char *bName2;
  int intNumber;
  float floatNumber;
  double doubleNumber;
  bool boolNumber;

  // These will be used for setting branches for comparisons.
//  int iNum[10][10];
  int iNum;
  float fNum;
  double dNum;
  bool bNum;

  // This string represents the rational/comparison operator
  std::string operatorStr;
};

class cuts
{
  public:
    cuts();
    // Set cuts
    void addCut(char *bName1, char *bName2, char *op);
    void addCut(char *bName1, int number, char *op);
    void addCut( char *bName1, float number,  char *op);
    void addCut( char *bName1, double number,  char *op);
    void addCut( char *bName1, bool boolean,  char *op);

    void setCore(muenalysis *coreIn);
    void setCuts(int file);
    void isCut(int file, int entry);


    // Get the cut
    cut getCut(int number){ return cutsVector[number]; };

    // Get number of cuts
    int getNCuts(){ return nCuts; };


    // Get the full cut string that can be used in e.g. root's ->Draw()
    std::string getCutString(int number);

  private:
    // Vector of cuts
    std::vector< cut > cutsVector;

    cut *initCut(char* sname1, char* op);

    // number of cuts
    int nCuts;
    muenalysis *core;
};

#endif
