// A helper util/script written by okumura-san.
// It can easily calculate towall, distance between two vertices etc.
// Author: Okumura-san
#include "skutil.h"

Double_t skutil::dist( TVector3 & x,  TVector3 & y ) {
  TVector3 a = x - y ;
  return a.Mag();
}

Double_t skutil::dist( float x[],  float y[] ) {
  TVector3 a( x[0], x[1], x[2] )  ;
  TVector3 b( y[0], y[1], y[2] )  ;
  return dist( a, b ) ;
}


Double_t skutil::distx( TVector3 & x,  TVector3 & y, TVector3 & dir  ) {
  TVector3 a = x - y ;
  Double_t d = dir.Mag() ; 
  return d>0. ? a.Dot( dir )/ d : 0. ;
}

Double_t skutil::distx( float x[],  float y[], float z[] ) {
  TVector3 a( x[0], x[1], x[2] )  ;
  TVector3 b( y[0], y[1], y[2] )  ;
  TVector3 c( z[0], z[1], z[2] )  ;
  return distx( a, b, c ) ;
}


Double_t skutil::distyz( TVector3 & x,  TVector3 & y, TVector3 & dir  ) {
  TVector3 a = x - y ;
  return a.Perp( dir ) ;
}

Double_t skutil::distyz( float x[],  float y[], float z[] ) {
  TVector3 a( x[0], x[1], x[2] )  ;
  TVector3 b( y[0], y[1], y[2] )  ;
  TVector3 c( z[0], z[1], z[2] )  ;
  return distyz( a, b, c ) ;
}


Double_t skutil::wallsk( TVector3 & pos ) {
  float a[3] ; 
  a[0] = pos.X();
  a[1] = pos.Y();
  a[2] = pos.Z();
  return wallsk( a ) ;
}

Double_t skutil::wallsk( float pos[] ) {
  double r = pos[0]*pos[0] + pos[1]*pos[1] ; 
  r = TMath::Sqrt( r ) ;
  double wall = skutil::RINTK - r ; 
  double z1 = skutil::ZPINTK - pos[2] ;
  double z2 = pos[2] - skutil::ZMINTK ;
  if ( z1<wall ) wall = z1 ; 
  if ( z2<wall ) wall = z2 ; 
  return wall ; 
}


Double_t skutil::towall( TVector3 & pos ,  TVector3 & dir  ) {
  float pos_[3], dir_[3] ; 
  pos_[0] = pos.X() ;
  pos_[1] = pos.Y() ;
  pos_[2] = pos.Z() ;
  dir_[0] = dir.X() ;
  dir_[1] = dir.Y() ;
  dir_[2] = dir.Z() ;
  return towall( pos_, dir_ ) ;
}

Double_t skutil::towall( double pos[], double dir[] )
{
	float pos_[3]={}, dir_[3]={};
	for( int i=0; i<3; i++ ){
		pos_[i]=(float)pos[i];
		dir_[i]=(float)dir[i];
	}
	return towall( pos_, dir_ );

}


Double_t skutil::towall( float pos[] ,  float dir[]  ) {
  Double_t hpos[3], rcir, cz ;
  Int_t hit = 0 ;
  if ( dir[2] < 0.0 ) {
    if ( pos[2] < ZMINTK ) {
      hit = 0 ;
    } else {
      cz = ( ZMINTK - pos[2] ) / dir[2] ; 
      hpos[0] = cz * dir[0] + pos[0] ; 
      hpos[1] = cz * dir[1] + pos[1] ; 
      hpos[2] = ZMINTK ; 
      rcir = sqrt( hpos[0]*hpos[0] + hpos[1]*hpos[1] ) ; 
      if ( rcir<RINTK ) hit = 3 ;         // hit on BOTTOM plane
    }
  } else if ( dir[2] > 0.0 ) {
    if ( pos[2] > ZPINTK ) {
      hit = 0 ;
    } else {
      cz = ( ZPINTK - pos[2] ) / dir[2] ; 
      hpos[0] = cz * dir[0] + pos[0] ; 
      hpos[1] = cz * dir[1] + pos[1] ; 
      hpos[2] = ZPINTK ; 
      rcir = sqrt( hpos[0]*hpos[0] + hpos[1]*hpos[1] ) ; 
      if ( rcir<RINTK ) hit = 2 ;          // hit on TOP plane
    }
  }
  if ( hit==0 ) {
    Double_t a0 = dir[0]*dir[0]+dir[1]*dir[1] ; 
    Double_t b0 = dir[0]*pos[0]+dir[1]*pos[1] ; 
    Double_t c0 = pos[0]*pos[0]+pos[1]*pos[1]-RINTK*RINTK ; 
    Double_t hanbe = b0*b0-c0*a0 ; 
    if ( hanbe >= 0. ) {
      Double_t cz1 = (-b0+sqrt(hanbe))/a0 ; 
      if ( cz1 > 0. ) {
	hpos[0] = cz1 * dir[0] + pos[0] ; 
	hpos[1] = cz1 * dir[1] + pos[1] ; 
	hpos[2] = cz1 * dir[2] + pos[2] ; 
	if ( hpos[2] <= ZPINTK && hpos[2] >= ZMINTK ) {
	  hit=1 ;
	}
      }
    }
  }
  Double_t towall ; 
  if ( hit==0 ) {
    towall = 0 ;
  } else {
    //    std::cout << "*** " << hpos[0] << " " << hpos[1] << " " << hpos[2] << std::endl ; 
    towall = sqrt( pow((pos[0]-hpos[0]),2.0) + pow((pos[1]-hpos[1]),2.0) + pow((pos[2]-hpos[2]),2.0) ) ; 
/*
        std::cout<< " *** (towall, hit) : (" << towall <<" , " << hit <<" ), (" <<
                    pos[0] << ","<< pos[1] <<"," << pos[2] <<"), ("<<
                    dir[0] << ","<< dir[1] <<"," << dir[2] <<") ***" <<std::endl;
*/

  }
  return towall ;
}

Double_t skutil::fromwall( TVector3 & pos ,  TVector3 & dir ) {
  TVector3 dir_( -dir.X(), -dir.Y(), -dir.Z() )   ;
  return towall( pos, dir_ ) ;
}

Double_t skutil::fromwall( float pos[] ,  float dir[] ) {
  float dir_[3] ; 
  dir_[0] = -dir[0] ; 
  dir_[1] = -dir[1] ; 
  dir_[2] = -dir[2] ; 
  return towall( pos, dir_ ) ;
}

//
//  get upper edge in which ratio of events contains
//
Double_t skutil::fitres( TH1 *h, float ratio ) {

  Int_t nbin = h->GetNbinsX() ;
  Double_t sum = h->Integral( 1, nbin+1 ) ;  // include overflow
  Double_t sum2 = h->Integral( 1, nbin ) ;  
  Double_t target = sum * ratio ; 
  Double_t x ; 
  if ( target > sum2 ) { 
    x = h->GetBinContent( nbin ) ; 
  } else if ( target < 0. ) {
    x = h->GetBinContent( 1 ) ; 
  } else {
    Int_t low=1, hi=nbin, mid ; 
    Double_t a, b, c ; 
    a = h->Integral( 1, low ) ; 
    c = h->Integral( 1, hi ) ; 
    while ( hi-low > 1 ) {
      mid = (low+hi)/2 ;
      b = h->Integral( 1, mid ) ; 
      if ( b < target ) { 
	low = mid  ;
	a = b ; 
      } else {
	hi = mid  ;
	c = b ; 
      }
    }
    Double_t x1 = h->GetXaxis()->GetBinCenter( low ) ;
    Double_t x2 = h->GetXaxis()->GetBinCenter( hi ) ;
    x = x1 + (target-a)/(c-a)*(x2-x1) ;
    //    cout << low << " " << hi << endl ;
    //    cout << x1 << " " << x2 << endl ;
    //    cout << a << " " << b << " " << c  << endl ;
  }
  return x ; 
}



// Convert TVector3 into floats and call towall(float) function
Double_t fqutil::fqwall( TVector3 & pos ){
  float pos_[3];
  pos_[0] = pos.X();
  pos_[1] = pos.Y();
  pos_[2] = pos.Z();

  return fqutil::fqwall(pos_);
}

// Convert double[] into floats and call towall(float) function
Double_t fqutil::fqwall( double pos[3] ){
  float pos_[3]={};
  for(int i = 0; i < 3; ++i)
    pos_[i] = (float)pos[i];

  return fqutil::fqwall(pos_);
}

// Calculate wall, fiTQun style
Double_t fqutil::fqwall( float pos[3] ){
    float Rmax = 1690.;
    float Zmax = 1810.;
    float rr   = sqrt(pos[0]*pos[0] + pos[1]*pos[1]);
    float absz = TMath::Abs(pos[2]);

    //check if vertex is outside tank
    float signflg = 1.;
    if (absz>Zmax) signflg = -1.;
    if (rr>Rmax)   signflg = -1.;

    //find min distance to wall
    float distz = TMath::Abs(Zmax-absz);
    float distr = TMath::Abs(Rmax-rr);

    float dwall = 0.0;
    if(distz <= distr) dwall = signflg*distz;
    else dwall = signflg*distr;
    return dwall;
}

// Convert TVector3's to floats and call fqtowall( float[] )
Double_t fqutil::fqtowall( TVector3 & pos ,  TVector3 & dir  ) {
  float pos_[3], dir_[3] ; 
  pos_[0] = pos.X() ;
  pos_[1] = pos.Y() ;
  pos_[2] = pos.Z() ;
  dir_[0] = dir.X() ;
  dir_[1] = dir.Y() ;
  dir_[2] = dir.Z() ;
  return fqutil::fqtowall( pos_, dir_ ) ;
}

// Convert doubles to floats and call fqtowall( float[] )
Double_t fqutil::fqtowall( double pos[], double dir[] ){
	float pos_[3]={}, dir_[3]={};
	for( int i=0; i<3; i++ ){
		pos_[i]=(float)pos[i];
		dir_[i]=(float)dir[i];
	}
	return fqutil::fqtowall( pos_, dir_ );
}

// Run fiTQun towall
Double_t fqutil::fqtowall( float pos[], float dir[] ){
  Double_t const R(1690);
  Double_t l_b(100000.0), H;
  Double_t l_t(100000.0);
  Double_t A, B, C, RAD;
  if(dir[0]!=0 || dir[1]!=0){
    A = (dir[0]*dir[0]+dir[1]*dir[1]);
    B = 2*(pos[0]*dir[0]+pos[1]*dir[1]);
    C = (pos[0]*pos[0]+pos[1]*pos[1]-R*R);
    RAD = (B*B) - (4*A*C);
    l_b = ((-1*B) + sqrt(RAD))/(2*A);
  }
  if (dir[2]==0){return l_b;}
  else if(dir[2]>0){H=1810;}
  if(dir[2]<0){H=-1810;}
  l_t=(H - pos[2])/dir[2];
  return  (l_t > l_b ? l_b:l_t);
}

