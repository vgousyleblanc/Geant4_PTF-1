/* CathodeSD
   Geant4 sensitive detector to process steps taken in the cathode of a PMT.

   Author: Blair Jamieson (May 2018)
 */

#ifndef _CathodeSD_hh_
#define _CathodeSD_hh_ 1

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "globals.hh"


class CathodeSD : public G4VSensitiveDetector {
public:
  
  CathodeSD( G4String aname );
  ~CathodeSD(){;}
  
  G4bool ProcessHits( G4Step * , G4TouchableHistory *  );
  //void   Initialize( G4HCofThisEvent * );
  //void   EndOfEvent( G4HCofThisEvent * );

  G4String get_name() const { return fName ; }
private:
  G4String fName;
};

/// report a helper for printing G4Step info
void PrintG4Step( G4Step* step );


#endif
