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

#ifndef OpNoviceDetectorConstruction_h
#define OpNoviceDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "R3600Geometry.hh"

//class OpNoviceDetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class OpNoviceDetectorConstruction : public G4VUserDetectorConstruction {
public:
  OpNoviceDetectorConstruction();
  virtual ~OpNoviceDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  void ConstructSDandField();
    
  void set_haswater( bool haswater ){
    std::cout<<"OpNoviceDetectorConstruction::set_haswater="<<haswater<<std::endl;
    tank_has_water = haswater; }
  void set_waterdepth( double waterdepth ){
    std::cout<<"OpNoviceDetectorConstruction::set_waterdepth="<<waterdepth/CLHEP::cm<<" cm"<<std::endl;
    water_height = waterdepth; }
  void set_dut( G4String dut ){ fDUT = dut; }
  void set_hasacrylic( bool hasacryl ){
    std::cout<<"OpNoviceDetectorConstruction::set_hasacrylic="<<hasacryl<<std::endl;
    r3600_hasacryl = hasacryl;}
  void set_glassthickness( double glassthick ){
    std::cout<<"OpNoviceDetectorConstruction::set_glassthickness="<<glassthick/CLHEP::mm<<" mm"<<std::endl;
    r3600_glassthick = glassthick; }
  void set_cathodethickness( double cathodethick ){
    std::cout<<"OpNoviceDetectorConstruction::set_cathodethickness="<<cathodethick/CLHEP::nm<<" nm"<<std::endl;
    r3600_caththick = cathodethick; }
  void set_acrylicthickness( double acrylthick ){
    std::cout<<"OpNoviceDetectorConstruction::set_acrylthick="<<acrylthick/CLHEP::mm<<" mm"<<std::endl;
    r3600_acrylthick = acrylthick; }

  const R3600Geometry * GetPMT() const { return fPMT; }
  
private:
  void ReadGeometry();

  //OpNoviceDetectorMessenger* fDetMessenger;
  
  G4double fExpHall_x;
  G4double fExpHall_y;
  G4double fExpHall_z;

  bool     tank_has_water;    // default true
  double   water_height; // default = 116.0 * CLHEP::cm; // height of water in the tank
  G4String fDUT;         // default "R3600"

  // parameters for R3600:
  bool     r3600_hasacryl; // default true
  double   r3600_glassthick;  // default 4mm
  double   r3600_caththick;   // default 30nm
  double   r3600_acrylthick;  // default 13 mm
  
  R3600Geometry * fPMT;

  // keep track of the world
  G4LogicalVolume * expHall_log;
  G4VPhysicalVolume * expHall_phys;

  G4VPhysicalVolume *  waterTank_phys;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*OpNoviceDetectorConstruction_h*/
