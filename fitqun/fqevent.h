#ifndef _fqevent_
#define _fqevent_
struct fqevent{

  // Certain SK parameters
  float wall; 

  // fiTQun 1R parameters
  float fq1rmom[150][6];
  float fq1rnll[150][6];

  int fqnse;

  // fiTQun MR parameters
  int fqnmrfit;
  int fqmrnring[150];
  float fqmrnll[150];
  float fqmrpid[150][6];

};
#endif
