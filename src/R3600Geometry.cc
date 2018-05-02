
#include "R3600Geometry.hh"
#include "MaterialsMap.hh"
#include "OpticalSurfaceDefinitions.hh"

#include "G4UnionSolid.hh"
#include "G4Ellipsoid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

R3600Geometry::R3600Geometry( G4LogicalVolume * parentvol, const G4ThreeVector & loc,
			      const G4ThreeVector & dir, bool add_water, bool add_acrylic ) :
  fParent(parentvol), fPMTId(0), fLoc(loc), fDir(dir.unit()), fInWater(add_water), fWithAcrylic(add_acrylic), fSumSolid(nullptr) {
  static int pmtcount=0;
  ++pmtcount; fPMTId = pmtcount;
  Init();
}

R3600Geometry::~R3600Geometry(){
  
}

void R3600Geometry::get_solid( G4VSolid & vol, G4RotationMatrix& rm, G4ThreeVector& loc ) {

  std::ostringstream os;
  int volcount=0;

  // check if solid volume was built already?
  if ( fSumSolid == nullptr ){
    //std::pair< std::string,G4VPhysicalVolume * > refpair = *(fVols.begin());
    //std::string refname = refpair.first;
    //G4VPhysicalVolume* refvol = refpair.second;
    G4VPhysicalVolume* refvol = fVols.begin()->second;
    G4VSolid* sumvol = refvol->GetLogicalVolume()->GetSolid();
    G4ThreeVector refloc = refvol->GetTranslation() - fLoc;
    for ( auto & curpair : fVols ){
      if ( refvol == curpair.second ) continue;
      G4VPhysicalVolume * curphysvol = curpair.second;
      G4VSolid* curvol = curphysvol->GetLogicalVolume()->GetSolid();
      G4ThreeVector curloc = curphysvol->GetTranslation() - refloc;
      reset(os);
      os << "R3600Sum_"<< fPMTId <<"_"<<++volcount;
      G4UnionSolid * cursumvol = new G4UnionSolid( os.str().c_str(), sumvol, curvol, 0, curloc );
      sumvol = cursumvol;
    }
    fSumSolid = sumvol;
  }

  vol = *fSumSolid;
  rm = *fRm;
  loc = fLoc;
  return;
}

void R3600Geometry::Init( ){
  std::ostringstream os;

  MaterialsMap & fMaterials = MaterialsMap::Get();
  OpticalSurfaceDefinitions & fSurfaces = OpticalSurfaceDefinitions::Get();
  
  // build rotation matrix
  fRm = new G4RotationMatrix;
  if ( fDir == G4ThreeVector(0.,0.,-1. ) ){
    fRm->rotateX( 180.0*CLHEP::deg );
  } else if ( fDir != G4ThreeVector(0.,0.,1. ) ){
    double norm= std::sqrt( fDir.x()*fDir.x() + fDir.y()*fDir.y() );
    G4ThreeVector nxy( fDir.y()/norm, -fDir.x()/norm, 0 );
    double rotang = std::acos( fDir.z() );
    fRm->rotate( rotang, nxy );
  }

  // PMT Glass Logical Volume.
  
  // build top glass
  os << "R3600EGTop_" << fPMTId;
  G4Ellipsoid* egtop = new G4Ellipsoid( os.str().c_str(), 274.0*CLHEP::mm, 274.0*CLHEP::mm, 210.0*CLHEP::mm, 0.0*CLHEP::mm, 210.0*CLHEP::mm );
  reset(os);
  os << "R3600EGBotop_" << fPMTId;
  G4Ellipsoid* egbotop = new G4Ellipsoid( os.str().c_str(), 274.0*CLHEP::mm, 274.0*CLHEP::mm, 180.0*CLHEP::mm, -153.0*CLHEP::mm, 0.0*CLHEP::mm );
  reset(os);
  // Union of top and bottom glass
  os << "R3600EGTopBotop_"<< fPMTId;
  G4UnionSolid * egtopbotop = new G4UnionSolid( os.str().c_str(), egtop, egbotop, 0, G4ThreeVector(0.,0.,0.) ); // no offset
  reset(os);
  // Build side of pmt glass
  os <<"R3600TGSide_"<<fPMTId;
  G4Tubs* tgside = new G4Tubs( os.str().c_str(), 0.0, 145.5*CLHEP::mm, 78.5*CLHEP::mm, 0., 2.0*CLHEP::pi );
  // side glass is at -231.5 mm from center of PMT
  // Union of top, bottom of top, and side glass
  reset(os);
  os << "R3600EGThreeParts_"<< fPMTId;
  G4UnionSolid * egthree = new G4UnionSolid( os.str().c_str(), egtopbotop, tgside, 0, G4ThreeVector(0.,0.,-231.5*CLHEP::mm) ); // 
  reset(os);
  os << "R3600EGBot_" << fPMTId;
  G4Ellipsoid* egbot = new G4Ellipsoid( os.str().c_str(), 141.5*CLHEP::mm, 141.5*CLHEP::mm, 90.0*CLHEP::mm, -90.0*CLHEP::mm*std::cos( 30*CLHEP::deg ), 0.0 );
  // bottom glass is at -310.0 mm from center of PMT
  // Union of top, bottom of top, side, and bottom glass
  reset(os);
  os << "R3600EGFourParts_"<< fPMTId;
  G4UnionSolid * egfour = new G4UnionSolid( os.str().c_str(), egthree, egbot, 0, G4ThreeVector(0.,0.,-310.0*CLHEP::mm) ); //
  // Pins part of glass
  reset(os);
  os <<"R3600TGPins_"<<fPMTId;
  G4Tubs* tgpins = new G4Tubs( os.str().c_str(), 0.0, 49.5*CLHEP::mm, 23.0*CLHEP::mm, 0., 2.0*CLHEP::pi );
  // pins are offset by -410.9 mm from center of PMT
  // Union of all PMT glas
  reset(os);
  os << "R3600EGlass_"<< fPMTId;
  G4UnionSolid * eglass = new G4UnionSolid( os.str().c_str(), egfour, tgpins, 0, G4ThreeVector(0.,0.,-410.0*CLHEP::mm) ); //
  // the PMT glass logical volume
  G4LogicalVolume * eglass_log = new G4LogicalVolume( eglass, fMaterials["pmtglass"], os.str().c_str() );

  // Place the glass
  // include the -210mm offset to put the top of the front glass at 0, instead of center of PMT dome
  G4ThreeVector offset = fLoc + G4ThreeVector( 0., 0., -210.0*CLHEP::mm );

  fVols[ "pmtglass" ] = new G4PVPlacement( fRm, offset, eglass_log, os.str().c_str(), fParent, false, 0 );
  fSurfaces.skin_surface( "pmtglass", fVols[ "pmtglass" ] );
  
  // PMT Vaccum Logical Volume.
  // It centers on PMT Glass, so placed "pmtvaccum" gets put inside PMT
  os << "R3600EVTop_" << fPMTId;
  G4Ellipsoid* evtop = new G4Ellipsoid( os.str().c_str(), 270.0*CLHEP::mm, 270.0*CLHEP::mm, 206.0*CLHEP::mm, 0.0*CLHEP::mm, 206.0*CLHEP::mm );
  reset(os);
  os << "R3600EVBotop_" << fPMTId;
  G4Ellipsoid* evbotop = new G4Ellipsoid( os.str().c_str(), 270.0*CLHEP::mm, 270.0*CLHEP::mm, 176.0*CLHEP::mm, -153.0*CLHEP::mm, 0.0*CLHEP::mm );
  reset(os);
  // Union of top and bottom of top vacuum
  os << "R3600EVTopBotop_"<< fPMTId;
  G4UnionSolid * evtopbotop = new G4UnionSolid( os.str().c_str(), evtop, evbotop, 0, G4ThreeVector(0.,0.,0.) ); // no offset
  reset(os);
  // Build side of pmt vacuum
  os <<"R3600TVSide_"<<fPMTId;
  G4Tubs* tvside = new G4Tubs( os.str().c_str(), 0.0, 141.5*CLHEP::mm, 78.5*CLHEP::mm, 0., 2.0*CLHEP::pi );
  // side vacuum is at -231.5 mm from center of PMT
  // Union of top, bottom of top, and side glass
  reset(os);
  os << "R3600EVThreeParts_"<< fPMTId;
  G4UnionSolid * evthree = new G4UnionSolid( os.str().c_str(), evtopbotop, tvside, 0, G4ThreeVector(0.,0.,-231.5*CLHEP::mm) ); // 
  reset(os);
  os << "R3600EVBot_" << fPMTId;
  G4Ellipsoid* evbot = new G4Ellipsoid( os.str().c_str(), 137.5*CLHEP::mm, 137.5*CLHEP::mm, 86.0*CLHEP::mm, -90.0*CLHEP::mm*std::cos( 30*CLHEP::deg ), 0.0 );
  // bottom glass is at -310.0 mm from center of PMT
  // Union of top, bottom of top, side, and bottom glass
  reset(os);
  os << "R3600EVFourParts_"<< fPMTId;
  G4UnionSolid * evfour = new G4UnionSolid( os.str().c_str(), evthree, evbot, 0, G4ThreeVector(0.,0.,-310.0*CLHEP::mm) ); //
  // Pins part of PMT vacuum
  reset(os);
  os <<"R3600TVPins_"<<fPMTId;
  G4Tubs* tvpins = new G4Tubs( os.str().c_str(), 0.0, 45.5*CLHEP::mm, 21.0*CLHEP::mm, 0., 2.0*CLHEP::pi );
  // pins are offset by -410.9+2 mm from center of PMT
  // Union of all PMT glas
  reset(os);
  os << "R3600EVacuum_"<< fPMTId;
  G4UnionSolid * evacuum = new G4UnionSolid( os.str().c_str(), evfour, tvpins, 0, G4ThreeVector(0.,0.,-408.0*CLHEP::mm) ); //
  /// the PMT glass logical volume
  G4LogicalVolume * evacuum_log = new G4LogicalVolume( evacuum, fMaterials["vacuum"], os.str().c_str() );

  // Place the vacuum inside the glass
  fVols[ "pmtvacuum" ] = new G4PVPlacement( 0, G4ThreeVector(0.,0.,0.), evacuum_log, os.str().c_str(), eglass_log, false, 0 );

  // and now the cathode. Place it in the vaccuum
  os << "R3600ECath1_" << fPMTId;
  G4Ellipsoid* ecath1 = new G4Ellipsoid( os.str().c_str(), 270.0*CLHEP::mm, 270.0*CLHEP::mm, 206.0*CLHEP::mm, 88.0*CLHEP::mm, 206.0*CLHEP::mm );
  reset(os);
  os << "R3600ECath2_" << fPMTId; // make cathode 1 um thick (way thicker than reality ~30-60 nm?)
  G4Ellipsoid* ecath2 = new G4Ellipsoid( os.str().c_str(), 270.0*CLHEP::mm-0.001*CLHEP::mm, 270.0*CLHEP::mm-0.001*CLHEP::mm, 206.0*CLHEP::mm-0.001*CLHEP::mm, 88.0*CLHEP::mm, 206.0*CLHEP::mm );
  // subtract from center of ellipse to make a void
  reset(os);
  os << "R3600ECath_"<<fPMTId;
  G4SubtractionSolid* ecath = new G4SubtractionSolid( os.str().c_str(), ecath1, ecath2 );
  // cathode logical volume, sets material of cathode
  G4LogicalVolume * ecath_log = new G4LogicalVolume( ecath, fMaterials["cathode"], os.str().c_str() );
  ecath_log->SetVisAttributes( G4VisAttributes( G4Color::Yellow() ) );
  reset(os);
  os <<"R3600PCath_"<<fPMTId;
  fVols[ "cathode" ] = new G4PVPlacement( 0, G4ThreeVector(0.,0.,0.), ecath_log, os.str().c_str(), evacuum_log, false, 0 );
  fSurfaces.skin_surface( "cathode", fVols[ "cathode" ] );
  
  // reflective coating inside PMT
  reset(os);
  os << "R3600ERBotop1_" << fPMTId;
  G4Ellipsoid* erbotop1 = new G4Ellipsoid( os.str().c_str(), 270.0*CLHEP::mm, 270.0*CLHEP::mm, 176.0*CLHEP::mm, -153.0*CLHEP::mm, 0.0*CLHEP::mm );
  reset(os);
  os << "R3600ERBotop2_" << fPMTId;
  G4Ellipsoid* erbotop2 = new G4Ellipsoid( os.str().c_str(), 269.999*CLHEP::mm, 269.999*CLHEP::mm, 175.999*CLHEP::mm, -153.0*CLHEP::mm, 0.0*CLHEP::mm );
  reset(os);
  os << "R3600ERBotop_"<< fPMTId;
  G4SubtractionSolid * ertopbot = new G4SubtractionSolid( os.str().c_str(), erbotop1, erbotop2 );
  reset(os);
  // Build side of pmt reflective coat
  os <<"R3600TRSide_"<<fPMTId;
  G4Tubs* trside = new G4Tubs( os.str().c_str(), 141.499*CLHEP::mm, 141.5*CLHEP::mm, 78.5*CLHEP::mm, 0., 2.0*CLHEP::pi );
  reset(os);
  os << "R3600ERTwoParts_"<< fPMTId;
  G4UnionSolid * ertwo = new G4UnionSolid( os.str().c_str(), ertopbot, trside, 0, G4ThreeVector(0.,0.,-231.5*CLHEP::mm) ); // 
  reset(os);
  os << "R3600ERBot1_" << fPMTId;
  G4Ellipsoid* erbot1 = new G4Ellipsoid( os.str().c_str(), 137.5*CLHEP::mm, 137.5*CLHEP::mm, 86.0*CLHEP::mm, -90.0*CLHEP::mm*std::cos( 30*CLHEP::deg ), 0.0 );
  reset(os);
  os << "R3600ERBot2_" << fPMTId;
  G4Ellipsoid* erbot2 = new G4Ellipsoid( os.str().c_str(), 137.499*CLHEP::mm, 137.499*CLHEP::mm, 85.999*CLHEP::mm, -90.0*CLHEP::mm*std::cos( 30*CLHEP::deg ), 0.0 );
  reset(os);
  os << "R3600ERBot_"<< fPMTId;
  G4SubtractionSolid * erbot = new G4SubtractionSolid( os.str().c_str(), erbot1, erbot2 );
  // bottom glass is at -310.0 mm from center of PMT
  // Union of top, bottom of top, side, and bottom glass
  reset(os);
  os << "R3600ERThreeParts_"<< fPMTId;
  G4UnionSolid * erthree = new G4UnionSolid( os.str().c_str(), ertwo, erbot, 0, G4ThreeVector(0.,0.,-310.0*CLHEP::mm) ); //

  // reflective coating logical volume, sets material
  G4LogicalVolume * ereflect_log = new G4LogicalVolume( erthree, fMaterials["Al"], os.str().c_str() );
  ereflect_log->SetVisAttributes( G4VisAttributes( G4Color::Blue() ) );

  reset(os);
  os <<"R3600Reflect_"<<fPMTId;
  fVols[ "reflective" ] = new G4PVPlacement( 0, G4ThreeVector(0.,0.,0.0), ereflect_log, os.str().c_str(), evacuum_log, false, 0 );
  fSurfaces.skin_surface( "reflector", fVols[ "reflective" ] );
  
  
  // and now the Metal dynodes
  reset(os);
  os <<"R3600FieldRim_"<<fPMTId;
  G4Tubs* tfrim = new G4Tubs( os.str().c_str(), 100.0*CLHEP::mm, 102.5*CLHEP::mm, 43.0*CLHEP::mm, 0., 2.0*CLHEP::pi );
  reset(os);
  os <<"R3600DynodeRim_"<<fPMTId;
  G4Tubs* tdrim = new G4Tubs( os.str().c_str(), 85.0*CLHEP::mm, 100.0*CLHEP::mm, 43.0*CLHEP::mm, 0., 2.0*CLHEP::pi );
  reset(os);
  os<<"R3600Union1_"<<fPMTId;
  G4UnionSolid * tdynunion1 = new G4UnionSolid(os.str().c_str(), tfrim, tdrim, 0, G4ThreeVector( 0.,0., -15.0*CLHEP::mm ) );
  reset(os);
  os<<"R3600Dynode_"<<fPMTId;
  G4Tubs* tdyn = new G4Tubs( os.str().c_str(), 0.0*CLHEP::mm, 85.0*CLHEP::mm, 43.0*CLHEP::mm, 0., 2.0*CLHEP::pi );
  reset(os);
  os<<"R3600Union2_"<<fPMTId;
  G4UnionSolid * tdynunion2 = new G4UnionSolid(os.str().c_str(), tdynunion1, tdyn, 0, G4ThreeVector( 0.,0., -52.0*CLHEP::mm ) );
  G4LogicalVolume* tdynunion2_log = new G4LogicalVolume( tdynunion2, fMaterials["pmtsteel"], os.str().c_str() );
  tdynunion2_log->SetVisAttributes( G4VisAttributes( G4Color::Red() ) );
  reset(os);
  os << "R3600PDynode_"<<fPMTId;
  //offset = fLoc + G4ThreeVector(0.,0.,-517.0*CLHEP::mm-112.0*CLHEP::mm );
  fVols["dynode"] = new G4PVPlacement( 0, G4ThreeVector(0.,0.,-112.0*CLHEP::mm), tdynunion2_log, os.str().c_str(), evacuum_log, false, 0 );
  fSurfaces.skin_surface( "pmtsteel", fVols[ "dynode" ] );

  

  if ( WithAcrylic() ) BuildAcrylic( );
  
  
  




}

void R3600Geometry::BuildAcrylic() {
  // here we add more placed volumes for the acrylic and FRP
  /// ...
  std::cout<<"R3600Geometry::BuildAcrylic not implemented yet"<<std::endl;
}

