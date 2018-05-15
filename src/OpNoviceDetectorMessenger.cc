#include "OpNoviceDetectorMessenger.hh"
#include "OpNoviceDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4SystemOfUnits.hh"

OpNoviceDetectorMessenger::OpNoviceDetectorMessenger( OpNoviceDetectorConstruction * OpDetCons )
  : G4UImessenger(), fDetectorConst( OpDetCons ) {

  fPTFDir = new G4UIdirectory("/ptf/");
  fPTFDir->SetGuidance("PMT Test Facility  Settings");

  fHasWater = new G4UIcmdWithABool("/ptf/haswater", this );
  fHasWater->SetGuidance("PTF Filled with water? (true/false)");
  fHasWater->SetDefaultValue("true");

  fWaterDepth = new G4UIcmdWithADoubleAndUnit("/ptf/waterdepth", this );
  fWaterDepth->SetGuidance("Water depth with units (must cover PMT or trouble)");
  fWaterDepth->SetUnitCategory( "Length" );
  fWaterDepth->SetDefaultValue( 116.0 );
  fWaterDepth->SetDefaultUnit( "cm" );
  
  fDUT = new G4UIcmdWithAString("/ptf/dut", this );
  fDUT->SetGuidance("PTF device under test (R3600,...)");
  fDUT->SetDefaultValue("R3600");

  fR3600HasAcryl = new G4UIcmdWithABool("/ptf/r3600hasacrylic", this );
  fR3600HasAcryl->SetGuidance("R3600 has acrylic cover (true/false)");
  fR3600HasAcryl->SetDefaultValue("true");

  fR3600GlassThick = new G4UIcmdWithADoubleAndUnit("/ptf/r3600glassthickness", this);
  fR3600GlassThick->SetGuidance("R3600 glass thickness with unit");
  fR3600GlassThick->SetUnitCategory( "Length" );
  fR3600GlassThick->SetDefaultValue( 4.0 );
  fR3600GlassThick->SetDefaultUnit( "mm" );

  fR3600CathThick = new G4UIcmdWithADoubleAndUnit("/ptf/r3600cathodethickness", this);
  fR3600CathThick->SetGuidance("R3600 cathode thickness with unit");
  fR3600CathThick->SetUnitCategory( "Length" );
  fR3600CathThick->SetDefaultValue( 30.0 );
  fR3600CathThick->SetDefaultUnit( "nm" );

  fR3600AcrylThick = new G4UIcmdWithADoubleAndUnit("/ptf/r3600acrylicthickness", this);
  fR3600AcrylThick->SetGuidance("R3600 acrylic cover thickness with unit");
  fR3600AcrylThick->SetUnitCategory( "Length" );
  fR3600AcrylThick->SetDefaultValue( 13.0 );
  fR3600GlassThick->SetDefaultUnit( "mm" );

  fUpdateCmd = new G4UIcommand("/ptf/update",this);
  fUpdateCmd->SetGuidance("Update the geometry with changed values.");
  fUpdateCmd->SetGuidance("Must be run before beamOn if geom has been changed.");
  
}


OpNoviceDetectorMessenger::~OpNoviceDetectorMessenger() {
  delete fPTFDir;
  delete fHasWater;
  delete fWaterDepth;
  delete fDUT;
  delete fR3600HasAcryl;
  delete fR3600GlassThick;
  delete fR3600CathThick;
  delete fR3600AcrylThick;
  delete fUpdateCmd;
}

void OpNoviceDetectorMessenger::SetNewValue( G4UIcommand* command, G4String newValue ){

  if ( command == fUpdateCmd ) {
    fDetectorConst->UpdateGeometry();
  } else  if ( command == fHasWater ){
    fDetectorConst->set_haswater( fHasWater->ConvertToBool( newValue ) );
    
  } else if ( command == fWaterDepth ){
    G4double depth = fWaterDepth->GetNewDoubleValue( newValue );
    std::cout<<"WaterDepth : newValue="<<newValue<<" depth="<<depth<<std::endl;
    fDetectorConst->set_waterdepth( depth );
    
    
  } else if ( command == fDUT ) {
    if ( newValue == "R3600" ) { // add to allowed values here...
      fDetectorConst->set_dut( newValue );
    }
    
  } else if ( command == fR3600HasAcryl ){
    fDetectorConst->set_hasacrylic( fR3600HasAcryl->ConvertToBool( newValue ) );
    
  } else if ( command == fR3600GlassThick ){
    G4double thickness = fR3600GlassThick->GetNewDoubleValue( newValue );
    std::cout<<"R3600GlassThick : newValue="<<newValue<<" thickness="<<thickness<<std::endl;
    fDetectorConst->set_glassthickness( thickness );
    
  } else if ( command == fR3600CathThick ){
    G4double thickness = fR3600CathThick->GetNewDoubleValue( newValue );
    std::cout<<"R3600CathThick : newValue="<<newValue<<" thickness="<<thickness<<std::endl;
    fDetectorConst->set_cathodethickness( thickness  );
    
  } else if ( command == fR3600AcrylThick ){
    G4double thickness = fR3600AcrylThick->GetNewDoubleValue( newValue );
    std::cout<<"R3600AcrylThick : newValue="<<newValue<<" thickness="<<thickness<<std::endl;
    fDetectorConst->set_acrylicthickness( thickness  );
    
  }

}
