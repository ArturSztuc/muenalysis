#ifndef _fqevent_
#define _fqevent_
struct fqevent{

  // Certain SK parameters
  float wall; 

  // 

  // Subevent information
  int fqnse;

  // fiTQun 1R parameters
  float fq1rmom[150][6];    // momentum
  float fq1rnll[150][6];    // nll
  float fq1rpos[150][7][3]; // v position
  float fq1rdir[150][7][3]; // v direction
  

  // fiTQun MR parameters
  int fqnmrfit;
  int fqmrnring[150];
  float fqmrnll[150];
  float fqmrpid[150][6];

  // User defined/custom variables
  float fqtowall[7]; // This will be towall for [0][7] fq1rpos/dir
  float fqwall[7];

};
#endif
