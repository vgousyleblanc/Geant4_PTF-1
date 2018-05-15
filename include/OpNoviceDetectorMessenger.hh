/* OpNoviceDetectorMessenger.hh
   Class to pass parameters from Geant4 UI to construction of PTF.
   Passes the macro paramters:
 
   /ptf/haswater              true/false 
   /ptf/waterdepth            value cm (must at least cover PMT, or trouble!)
   /ptf/dut                   type of device under test (R3600, ... others to be added ...)
   /ptf/r3600hasacrylic       true/false (to add or remove acrylic cover)
   /ptf/r3600glassthickness   value nm
   /ptf/r3600cathodethickness value nm
   /ptf/r3600acrylicthickness value nm

   Author: Blair Jamieson (May 2018)
*/

#ifndef OpNoviceDetectorMessenger_h
#define OpNoviceDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class OpNoviceDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithABool;


class OpNoviceDetectorMessenger : public G4UImessenger {
public:
  OpNoviceDetectorMessenger( OpNoviceDetectorConstruction* );
  virtual ~OpNoviceDetectorMessenger();
  virtual void SetNewValue( G4UIcommand*, G4String );

private:
  OpNoviceDetectorConstruction * fDetectorConst;
  G4UIdirectory                * fPTFDir;
  G4UIcmdWithABool             * fHasWater;
  G4UIcmdWithADoubleAndUnit    * fWaterDepth;
  G4UIcmdWithAString           * fDUT;
  G4UIcmdWithABool             * fR3600HasAcryl;
  G4UIcmdWithADoubleAndUnit    * fR3600GlassThick;
  G4UIcmdWithADoubleAndUnit    * fR3600CathThick;
  G4UIcmdWithADoubleAndUnit    * fR3600AcrylThick;
  G4UIcommand                  * fUpdateCmd;
};

#endif
