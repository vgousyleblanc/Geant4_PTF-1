
#include "PhotonCathodeHit.hh"

PhotonCathodeHit::PhotonCathodeHit( const G4ThreeVector& pos,  
				    const G4ThreeVector& dir,  
				    double time,               
				    double energy,             
				    unsigned PMTnum ) 
  : G4VHit(), fPos( pos ), fDir( dir ), fTime( time ),
    fEne( energy ), fPMTNum( PMTnum )
{

}
