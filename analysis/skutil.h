// A helper util/script written by okumura-san.
// It can easily calculate towall, distance between two vertices etc.
// Author: Okumura-san
#ifndef SKUTIL_HH_
#define SKUTIL_HH_

#include <iostream> 
#include <fstream> 
#include <cmath> 

#include "TROOT.h"
#include "TVector3.h"
#include "TH1.h"
#include "TMath.h"


namespace skutil {

  //
  // SK tank parameter from geotnk.h 
  //
  const Double_t DITKTK = 3930. ; 
  const Double_t HITKTK=4140. ; 
  const Double_t DIINTK=3380. ; 
  const Double_t HIINTK=3620. ; 
  const Double_t HIWAL=3605.7 ; 
  const Double_t TTATTK=260. ; 
  const Double_t TBATTK=260. ; 
  const Double_t TWATTK=275. ; 
  const Double_t RTKTK = DITKTK/2. ; 
  const Double_t ZPTKTK=HITKTK/2. ; 
  const Double_t ZMTKTK=-HITKTK/2. ; 
  const Double_t RINTK=DIINTK/2. ; 
  const Double_t ZPINTK=HIINTK/2. ; 
  const Double_t ZMINTK=-HIINTK/2. ; 
  const Double_t ZPWAL=HIWAL/2. ; 
  const Double_t ZMWAL=-HIWAL/2. ; 
  const Double_t ZCNTTK=HITKTK/2. ;
  const Double_t RMED  = 55. ;
  const Double_t ZMED  = 55. ;
  

  Double_t dist( TVector3 &,  TVector3 & ) ;
  Double_t dist( float [],  float [] ) ;

  Double_t distx( TVector3 &,  TVector3 & , TVector3 & ) ;
  Double_t distx( float [],  float [], float [] ) ;

  Double_t distyz( TVector3 &,  TVector3 & , TVector3 & ) ;
  Double_t distyz( float [],  float [], float [] ) ;

  Double_t wallsk( TVector3 & ) ;
  Double_t wallsk( float [] ) ;

  Double_t towall( TVector3 &,  TVector3 &  ) ;
  Double_t towall( float [],  float [] ) ;
  Double_t towall( double [],  double [] ) ;

  Double_t fromwall( TVector3 &,  TVector3 & ) ;
  Double_t fromwall( float [],  float [] ) ;

  Double_t fitres( TH1 *,  float ) ;

}

namespace fqutil {
  Double_t fqwall( TVector3 & );
  Double_t fqwall( double pos[3] );
  Double_t fqwall( float pos[3] );

  Double_t fqtowall( TVector3 &, TVector3 & );
  Double_t fqtowall( double [], double [] );
  Double_t fqtowall( float [], float [] );
}


#endif

