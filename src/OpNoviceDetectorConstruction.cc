//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//#include "OpNoviceDetectorMessenger.hh"
#include "OpNoviceDetectorConstruction.hh"


#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SDManager.hh"

#include "MaterialsMap.hh"
#include "OpticalSurfaceDefinitions.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceDetectorConstruction::OpNoviceDetectorConstruction()
 : G4VUserDetectorConstruction()
{
  // set defaults
  fExpHall_x = fExpHall_y = fExpHall_z = 20.0*m;
  //fDetMessenger = new OpNoviceDetectorMessenger( this );
  tank_has_water = true;
  water_height = 116.0*CLHEP::cm;
  fDUT = "R3600";
  r3600_hasacryl = true;
  r3600_glassthick = 4.0*CLHEP::mm;
  r3600_caththick = 30.0*CLHEP::nm;
  r3600_acrylthick = 13.0*CLHEP::mm;
  fPMT=nullptr;
  expHall_log=nullptr;
  expHall_phys=nullptr;

  ReadGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceDetectorConstruction::~OpNoviceDetectorConstruction(){
  if ( fPMT!=nullptr ) delete fPMT;
}


void OpNoviceDetectorConstruction::ConstructSDandField(){
  //
  //std::cout<<"fPMT->fSD = "<<fPMT->fSD<<std::endl;
  fPMT->attachSD(0); //PMT 0!
  G4SDManager::GetSDMpointer()->ListTree();
}

/*
void OpNoviceDetectorConstruction::UpdateGeometry() {

// clean-up previous geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  fPMT->Disable();

  expHall_log->RemoveDaughter( waterTank_phys );
  delete waterTank_phys;
  
  // G4PhysicalVolumeStore::GetInstance()->Clean();
  //G4LogicalVolumeStore::GetInstance()->Clean();
  //G4SolidStore::GetInstance()->Clean();
  //G4SDManager::GetSDMpointer()->ListTree();
  // try to inactivate any exiting sensitive detectors
  //const G4SDStructure * sds = G4SDManager::GetSDMpointer->GetTreeTop();


  
  //fPMT->disableSD( true );
  
  //G4LogicalSkinSurface::CleanSurfaceTable();
  //G4LogicalBorderSurface::CleanSurfaceTable();
  //G4SurfaceProperty::CleanSurfacePropertyTable();

  //define new one
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());//ConstructDetector()
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  //G4RunManager::GetRunManager()->ReinitializeGeometry();
  
  G4SDManager::GetSDMpointer()->ListTree();

  std::cout<<"The Geometry has been modified"<<std::endl;
}
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* OpNoviceDetectorConstruction::Construct()
{
  std::cout<<"OpNoviceDetectorConstruction::Construct()"<<std::endl;
  
  MaterialsMap & fMaterials = MaterialsMap::Get();
  OpticalSurfaceDefinitions & fSurfaces = OpticalSurfaceDefinitions::Get();
  
  std::cout<<"Got materials map"<<std::endl;
  // Air
  //
  G4Material* air = fMaterials[ "air"];



  // The experimental Hall
  // only build it once.
  if ( expHall_log == nullptr ){
    G4Box* expHall_box = new G4Box("World",fExpHall_x,fExpHall_y,fExpHall_z);
    
    expHall_log = new G4LogicalVolume(expHall_box,air,"World",0,0,0);
    
    expHall_phys = new G4PVPlacement(0,G4ThreeVector(),expHall_log,"World",0,false,0);
    
  }
  //if ( reallyconstruct == false ) return expHall_phys;
  
  
  // Water
  //
  G4Material* water = fMaterials[ "water" ];

  // Black water (or air) tank
  G4Material* black_plastic = fMaterials[ "blacksheet" ];
  
  //
  // ------------- Volumes --------------
  
  const double tank_height =  2.0 * 63.818 * CLHEP::cm;

  const double pmt_offset_to_rim = 35.0 * CLHEP::cm; // distance from top of tank of top of PMT (made up number!)
  const double water_offsetz = ( tank_height - water_height )/2.0;
  // PMT offset in water volume
  const double pmt_offsetz = ( tank_height/2.0 - pmt_offset_to_rim ) +water_offsetz;

  // Calculate how much to offset the water tank, so that the front of the PMT glass ends up at 0.,0.,0.:
  const double tank_world_offset = -pmt_offsetz+water_offsetz; //-240.0 * CLHEP::mm;

  std::cout<<"pmt_offsetz="<<pmt_offsetz<<std::endl;

  // The Water Tank
  //
  G4Tubs* waterTank_solid = new G4Tubs("Tank",
				       60.96 * CLHEP::cm,
				       64.456 * CLHEP::cm ,
				       tank_height/2.0,
				       0.,
				       2.0 * CLHEP::pi );
  // And add a rim and bottom
  G4Tubs* waterTank_rim = new G4Tubs("TankRim",60.96 * CLHEP::cm, 66.827 * CLHEP::cm, 1.945 * CLHEP::cm, 0., 2.0 * CLHEP::pi );
  G4Tubs* waterTank_bottom = new G4Tubs("TankBot",0.0 * CLHEP::cm, 60.96 * CLHEP::cm, 1.0 * CLHEP::cm, 0., 2.0 * CLHEP::pi );
  G4UnionSolid * waterTank_tub = new G4UnionSolid( "waterTank_tub", waterTank_solid, waterTank_rim, 0, G4ThreeVector( 0., 0., (64.456-1.945)*CLHEP::cm ) );
  G4UnionSolid * waterTank_tot = new G4UnionSolid( "waterTank_tot", waterTank_tub, waterTank_bottom, 0, G4ThreeVector( 0., 0., (-64.456+1.0)*CLHEP::cm ) );
  G4LogicalVolume* waterTank_log = new G4LogicalVolume(waterTank_tot,black_plastic,"Tank",0,0,0);
  waterTank_phys = new G4PVPlacement(0,G4ThreeVector(0.,0.,tank_world_offset),waterTank_log,"Tank", expHall_log,false,0);
  fSurfaces.skin_surface( "blacksheet", waterTank_phys );
  waterTank_log->SetVisAttributes( G4VisAttributes( G4Color::Brown() ) );
  
  // Fill Water tank with Air first
  //G4Tubs * Tankair_tub = new G4Tubs("TankAir",0.0, 60.325 * CLHEP::cm, 63.818 * CLHEP::cm , 0., 2.0 * CLHEP::pi );
  //G4LogicalVolume * Tankair_log = new G4LogicalVolume( Tankair_tub, air, "TankAir_log" );
  //G4VPhysicalVolume* Tankair_phys = new G4PVPlacement( 0, G4ThreeVector(0.,0.,0.635*CLHEP::cm), Tankair_log, "Tankair_phys", waterTank_log, false, 0 );
  // give the air a surface property
  
  
  // Add Water to the tank.
  G4Tubs * Tankwater_tub = new G4Tubs("TankWater",0.0, 60.325 * CLHEP::cm, water_height / 2.0, 0., 2.0 * CLHEP::pi );
  G4LogicalVolume * Tankwater_log = new G4LogicalVolume( Tankwater_tub, tank_has_water ? water : air, "Tankwater_log" );
  //  G4VPhysicalVolume* Tankwater_phys = new G4PVPlacement( 0, G4ThreeVector( 0., 0., -water_offsetz), Tankwater_log, "Tankwater_phys", waterTank_log, false, 0 );
  G4VPhysicalVolume* Tankwater_phys = new G4PVPlacement( 0, G4ThreeVector( 0., 0., tank_world_offset-water_offsetz), Tankwater_log, "Tankwater_phys", expHall_log, false, 0 );
  if ( tank_has_water ){
    fSurfaces.skin_surface( "water", Tankwater_phys );
    Tankwater_log->SetVisAttributes( G4VisAttributes( G4Color::Cyan() ) );
  }
  
  // put a pmt somewhere in geometry
  fPMT = new R3600Geometry( Tankwater_log,
			    G4ThreeVector( 0., 0., pmt_offsetz ),
			    G4ThreeVector( 0., 0., 1. ),
			    r3600_hasacryl,
			    r3600_acrylthick,
			    r3600_glassthick,
			    r3600_caththick );

  //fPMT->attachSD();
			    
  //fPMT = new R3600Geometry( waterTank_log, G4ThreeVector( 0., -200.*cm, 0. ),  G4ThreeVector( 0., 1., 0. ), true, false );
  //fPMT = new R3600Geometry( waterTank_log, G4ThreeVector( 30.*cm, 0., 0. ),  G4ThreeVector( -1, 0., 0. ), true, false );
  //fPMT = new R3600Geometry( waterTank_log, G4ThreeVector( -140.*cm, -140.0*cm, 0. ),  G4ThreeVector( 0.707, 0.707, 0. ), true, false );

  if (0){
  // make circle of PMTs at 4.0 m
  for (int ipmt = 0 ; ipmt < 18; ++ipmt ){
    double ang = ipmt * 360.0*CLHEP::deg / 18;
    double x = 400.*cm * std::cos( ang );
    double y = 400.*cm * std::sin( ang );
    double dx = -std::cos(ang);
    double dy = -std::sin(ang);
    //fPMT = new R3600Geometry( waterTank_log, G4ThreeVector( x, y, 0. ),  G4ThreeVector( dx, dy, 0. ), true, false );
  }
  }
//always return the physical World
  return expHall_phys;
}

void apply_unit( double & val, const std::string& unit ){
  if ( unit == "nm" ){
    val *= CLHEP::nm;
  } else if ( unit == "um" ){
    val *= CLHEP::um;
  } else if ( unit == "mm" ){
    val *= CLHEP::mm;
  } else if ( unit == "cm" ){
    val *= CLHEP::cm;
  } else if ( unit == "m" ){
    val *= CLHEP::m;
  } else {
    std::cout<<"apply_unit "<<unit<<", unknown unit, not applying."<<std::endl;
  }
}

void OpNoviceDetectorConstruction::ReadGeometry(){

  std::cout<<"OpNoviceDetectorConstruction::ReadGeometry() starting "<<std::endl;
  
  std::ifstream infile( "ptf-geom.txt" );

  if ( !infile ) {
    std::cout<<"Could not open ptf-geom.txt, using default geometry"<<std::endl;
    return;
  }

  std::string line;
  while ( std::getline( infile, line ) ){
    std::cout<<"Processing line: "<<line<<std::endl;
    std::istringstream is( line );
    std::string tag;

    is >> tag;
    if ( tag == "#" ){
      continue;
    } else if ( tag == "haswater" ) {
      std::string mybool;
      is >> mybool;
      if ( mybool == "true" ) tank_has_water = true;
      else if ( mybool == "false" ) tank_has_water = false;
      else if ( mybool == "0" ) tank_has_water = false;
      else tank_has_water = true;
      std::cout<<"OpNoviceDetectorConstruction::ReadGeometry tank_has_water = "<<tank_has_water<<std::endl;
      
    } else if ( tag == "r3600hasacrylic" ){
      std::string mybool;
      is >> mybool;
      if ( mybool == "true" ) r3600_hasacryl = true;
      else if ( mybool == "false" ) r3600_hasacryl = false;
      else if ( mybool == "0" ) r3600_hasacryl = false;
      else r3600_hasacryl = true;
      std::cout<<"OpNoviceDetectorConstruction::ReadGeometry r3600_hasacryl = "<<r3600_hasacryl<<std::endl;

    } else if ( tag == "r3600glassthickness" ){
      std::string unit;
      is >> r3600_glassthick >> unit;
      apply_unit( r3600_glassthick, unit );
      std::cout<<"OpNoviceDetectorConstruction::ReadGeometry r3600_glassthick = "<<r3600_glassthick/CLHEP::mm<<" mm"<<std::endl;

    } else if ( tag == "r3600cathodethickness" ){
      std::string unit;
      is >> r3600_caththick >> unit;
      apply_unit( r3600_caththick, unit );
      std::cout<<"OpNoviceDetectorConstruction::ReadGeometry r3600_caththick = "<<r3600_caththick/CLHEP::nm<<" nm"<<std::endl;


    } else if ( tag == "r3600acrylicthickness" ){
      std::string unit;
      is >> r3600_acrylthick >> unit;
      apply_unit( r3600_acrylthick, unit );
      std::cout<<"OpNoviceDetectorConstruction::ReadGeometry r3600_acrylthick = "<<r3600_acrylthick/CLHEP::mm<<" mm"<<std::endl;


    }
  }
  std::cout<<"OpNoviceDetectorConstruction::ReadGeometry() ending "<<std::endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
