
#include "OpticalSurfaceDefinitions.hh"
#include "G4MaterialPropertiesTable.hh"

std::vector< G4LogicalBorderSurface * > OpticalSurfaceDefinitions::fBorders;
std::vector< G4LogicalSkinSurface * > OpticalSurfaceDefinitions::fSkins;
std::map< std::string, G4OpticalSurface * > OpticalSurfaceDefinitions::fSurfaces;
OpticalSurfaceDefinitions * OpticalSurfaceDefinitions::fOpSurfaceDefs = nullptr;

OpticalSurfaceDefinitions& OpticalSurfaceDefinitions::Get(){
  if ( fOpSurfaceDefs == nullptr ) OpticalSurfaceDefinitions();
  return * fOpSurfaceDefs;
}

G4OpticalSurface * OpticalSurfaceDefinitions::operator[]( const std::string& key ) const {
  if ( fSurfaces[ key ] == nullptr ){
    std::cerr<<"OpticalSurfaceDefinitions::operator[] key="<<key<<" not found"<<std::endl;
    throw Invalid();
  }
  return fSurfaces[ key ];
}

 
void OpticalSurfaceDefinitions::skin_surface( const std::string& surface_name,
					      G4VPhysicalVolume* volume ){
  G4OpticalSurface * surf = fSurfaces[ surface_name ];
  if ( surf == nullptr ) {
    std::cerr<<"OpticalSurfaceDefinitions::skin_surface name="<<surface_name<<" not found"<<std::endl;
    throw Invalid();
  }
  G4LogicalVolume * lvol = volume->GetLogicalVolume();
  fSkins.push_back( new G4LogicalSkinSurface( surface_name, lvol, surf ) );
}

 
void OpticalSurfaceDefinitions::border_surface( const std::string& surface_name,
						G4VPhysicalVolume* volume1,
						G4VPhysicalVolume* volume2  ){
  G4OpticalSurface * surf = fSurfaces[ surface_name ];
  if ( surf == nullptr ) {
    std::cerr<<"OpticalSurfaceDefinitions::border_surface name="<<surface_name<<" not found"<<std::endl;
    throw Invalid();
  }
  fBorders.push_back( new G4LogicalBorderSurface( surface_name, volume1, volume2, surf ) );
}

OpticalSurfaceDefinitions::OpticalSurfaceDefinitions(){
  if ( fOpSurfaceDefs == nullptr ) {
    BuildSurfaces();
    fOpSurfaceDefs  = this;
  }
}

void OpticalSurfaceDefinitions::BuildSurfaces() {
  
   const G4int NUM = 2;
   const G4int NUMENTRIES_water=60;
   const G4int NUMENTRIES_acrylic=306;
   
   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** water *************************************************************************
   fSurfaces[ "water" ] = new G4OpticalSurface("Water");
   fSurfaces[ "water" ]->SetType(dielectric_dielectric);
   fSurfaces[ "water" ]->SetModel(unified); 
   fSurfaces[ "water" ]->SetFinish(ground);

   fSurfaces[ "water" ]->SetSigmaAlpha(0.001);
   G4double ENERGY_water[NUMENTRIES_water] =
     { 1.56962*CLHEP::eV, 1.58974*CLHEP::eV, 1.61039*CLHEP::eV, 1.63157*CLHEP::eV, 
       1.65333*CLHEP::eV, 1.67567*CLHEP::eV, 1.69863*CLHEP::eV, 1.72222*CLHEP::eV, 
       1.74647*CLHEP::eV, 1.77142*CLHEP::eV, 1.7971*CLHEP::eV,  1.82352*CLHEP::eV, 
       1.85074*CLHEP::eV, 1.87878*CLHEP::eV, 1.90769*CLHEP::eV, 1.93749*CLHEP::eV, 
       1.96825*CLHEP::eV, 1.99999*CLHEP::eV, 2.03278*CLHEP::eV, 2.06666*CLHEP::eV,
       2.10169*CLHEP::eV, 2.13793*CLHEP::eV, 2.17543*CLHEP::eV, 2.21428*CLHEP::eV, 
       2.25454*CLHEP::eV, 2.29629*CLHEP::eV, 2.33962*CLHEP::eV, 2.38461*CLHEP::eV, 
       2.43137*CLHEP::eV, 2.47999*CLHEP::eV, 2.53061*CLHEP::eV, 2.58333*CLHEP::eV, 
       2.63829*CLHEP::eV, 2.69565*CLHEP::eV, 2.75555*CLHEP::eV, 2.81817*CLHEP::eV, 
       2.88371*CLHEP::eV, 2.95237*CLHEP::eV, 3.02438*CLHEP::eV, 3.09999*CLHEP::eV,
       3.17948*CLHEP::eV, 3.26315*CLHEP::eV, 3.35134*CLHEP::eV, 3.44444*CLHEP::eV, 
       3.54285*CLHEP::eV, 3.64705*CLHEP::eV, 3.75757*CLHEP::eV, 3.87499*CLHEP::eV, 
       3.99999*CLHEP::eV, 4.13332*CLHEP::eV, 4.27585*CLHEP::eV, 4.42856*CLHEP::eV, 
       4.59258*CLHEP::eV, 4.76922*CLHEP::eV, 4.95999*CLHEP::eV, 5.16665*CLHEP::eV, 
       5.39129*CLHEP::eV, 5.63635*CLHEP::eV, 5.90475*CLHEP::eV, 6.19998*CLHEP::eV };   
    G4double RINDEX_water[NUMENTRIES_water] = // M Fechner : new ; define the water refraction index using refsg.F 
     {1.32885, 1.32906, 1.32927, 1.32948,
      1.3297,  1.32992, 1.33014, 1.33037,
      1.3306,  1.33084, 1.33109, 1.33134,
      1.3316,  1.33186, 1.33213, 1.33241,
      1.3327,  1.33299, 1.33329, 1.33361,
      1.33393, 1.33427, 1.33462, 1.33498,
      1.33536, 1.33576, 1.33617, 1.3366,
      1.33705, 1.33753, 1.33803, 1.33855,
      1.33911, 1.3397,  1.34033, 1.341,
      1.34172, 1.34248, 1.34331, 1.34419,
      1.34515, 1.3462,  1.34733, 1.34858,
      1.34994, 1.35145, 1.35312, 1.35498,
      1.35707, 1.35943, 1.36211, 1.36518,
      1.36872, 1.37287, 1.37776, 1.38362,
      1.39074, 1.39956, 1.41075, 1.42535};
    G4double ABWFF = 1.0;
    G4double ABSORPTION_water[NUMENTRIES_water] = //T. Akiri: Values from Skdetsim 
      { 16.1419*CLHEP::cm*ABWFF,  18.278*CLHEP::cm*ABWFF, 21.0657*CLHEP::cm*ABWFF, 24.8568*CLHEP::cm*ABWFF,
	30.3117*CLHEP::cm*ABWFF, 38.8341*CLHEP::cm*ABWFF, 54.0231*CLHEP::cm*ABWFF, 81.2306*CLHEP::cm*ABWFF,
	120.909*CLHEP::cm*ABWFF, 160.238*CLHEP::cm*ABWFF, 193.771*CLHEP::cm*ABWFF, 215.017*CLHEP::cm*ABWFF,
	227.747*CLHEP::cm*ABWFF,  243.85*CLHEP::cm*ABWFF, 294.036*CLHEP::cm*ABWFF, 321.647*CLHEP::cm*ABWFF,
	342.81*CLHEP::cm*ABWFF,  362.827*CLHEP::cm*ABWFF, 378.041*CLHEP::cm*ABWFF, 449.378*CLHEP::cm*ABWFF,
        739.434*CLHEP::cm*ABWFF, 1114.23*CLHEP::cm*ABWFF, 1435.56*CLHEP::cm*ABWFF, 1611.06*CLHEP::cm*ABWFF,
	1764.18*CLHEP::cm*ABWFF, 2100.95*CLHEP::cm*ABWFF,  2292.9*CLHEP::cm*ABWFF, 2431.33*CLHEP::cm*ABWFF,
	3053.6*CLHEP::cm*ABWFF,  4838.23*CLHEP::cm*ABWFF, 6539.65*CLHEP::cm*ABWFF, 7682.63*CLHEP::cm*ABWFF,
	9137.28*CLHEP::cm*ABWFF, 12220.9*CLHEP::cm*ABWFF, 15270.7*CLHEP::cm*ABWFF, 19051.5*CLHEP::cm*ABWFF,
	23671.3*CLHEP::cm*ABWFF, 29191.1*CLHEP::cm*ABWFF, 35567.9*CLHEP::cm*ABWFF,   42583*CLHEP::cm*ABWFF,
        49779.6*CLHEP::cm*ABWFF, 56465.3*CLHEP::cm*ABWFF,   61830*CLHEP::cm*ABWFF, 65174.6*CLHEP::cm*ABWFF,
	66143.7*CLHEP::cm*ABWFF,   64820*CLHEP::cm*ABWFF,   61635*CLHEP::cm*ABWFF, 57176.2*CLHEP::cm*ABWFF,
	52012.1*CLHEP::cm*ABWFF, 46595.7*CLHEP::cm*ABWFF, 41242.1*CLHEP::cm*ABWFF, 36146.3*CLHEP::cm*ABWFF,
	31415.4*CLHEP::cm*ABWFF, 27097.8*CLHEP::cm*ABWFF, 23205.7*CLHEP::cm*ABWFF, 19730.3*CLHEP::cm*ABWFF,
	16651.6*CLHEP::cm*ABWFF, 13943.6*CLHEP::cm*ABWFF, 11578.1*CLHEP::cm*ABWFF, 9526.13*CLHEP::cm*ABWFF };
   G4double ENERGY_blacksheet[NUM] = { 1.4*CLHEP::eV,6.2*CLHEP::eV};
   G4double CSL_water   [NUM] = { 0.99, 0.99 };  //Specular Lobe fraction
   G4double CSS_water   [NUM] = { 0.01, 0.01 };  //Specular Scatter fraction
   G4double CBS_water   [NUM] = { 0.0, 0.0 };  //backscatter fraction
   G4MaterialPropertiesTable *mySTWa = new G4MaterialPropertiesTable();
   mySTWa->AddProperty("ABSLENGTH",             ENERGY_water,      ABSORPTION_water, NUMENTRIES_water);
   mySTWa->AddProperty("RINDEX",                ENERGY_water,      RINDEX_water, NUMENTRIES_water );
   mySTWa->AddProperty("SPECULARLOBECONSTANT",  ENERGY_blacksheet, CSL_water,    NUM);
   mySTWa->AddProperty("SPECULARSPIKECONSTANT", ENERGY_blacksheet, CSS_water,    NUM);
   mySTWa->AddProperty("BACKSCATTERCONSTANT",   ENERGY_blacksheet, CBS_water,    NUM);
   fSurfaces[ "water" ]->SetMaterialPropertiesTable(mySTWa);

   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** air *************************************************************************
   fSurfaces[ "air" ] = new G4OpticalSurface("Air");
   fSurfaces[ "air" ]->SetType(dielectric_dielectric);
   fSurfaces[ "air" ]->SetModel(unified);
   fSurfaces[ "air" ]->SetFinish(polished);
   G4double RINDEX_air[NUMENTRIES_water] = 
     { 1.000275, 1.000275, 1.000275, 1.000275,
       1.000275, 1.000275, 1.000275, 1.000275,
       1.000276, 1.000276, 1.000276, 1.000276,
       1.000276, 1.000276, 1.000276, 1.000276,
       1.000277, 1.000277, 1.000277, 1.000277,
       1.000277, 1.000277, 1.000277, 1.000277,
       1.000278, 1.000278, 1.000278, 1.000278,
       1.000279, 1.000279, 1.000279, 1.000279,
       1.000280, 1.000280, 1.000280, 1.000280,
       1.000281, 1.000281, 1.000281, 1.000281,
       1.000282, 1.000283, 1.000284, 1.000285,
       1.000286, 1.000287, 1.000288, 1.000290,
       1.000291, 1.000292, 1.000293, 1.000295,
       1.000296, 1.000299, 1.000301, 1.000304,
       1.000308, 1.000312, 1.000316, 1.000320};
   G4MaterialPropertiesTable *mySTAir = new G4MaterialPropertiesTable();
   mySTAir->AddProperty("RINDEX",                ENERGY_water,      RINDEX_air, NUMENTRIES_water );
   fSurfaces[ "air" ]->SetMaterialPropertiesTable(mySTAir);
   
   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** blacksheet ***********************************************************************************
   fSurfaces[ "blacksheet" ] = new G4OpticalSurface("BlacksheetSurface");
   fSurfaces[ "blacksheet" ]->SetType(dielectric_dielectric);
   fSurfaces[ "blacksheet" ]->SetModel(unified); 
   fSurfaces[ "blacksheet" ]->SetFinish(ground);
   fSurfaces[ "blacksheet" ]->SetSigmaAlpha(0.1);
   
   G4double RINDEX_blacksheet[NUM] = { 1.6, 1.6 };
   G4double CSL_blacksheet   [NUM] = { 0.3, 0.3 };  //Specular Lobe fraction
   G4double CSS_blacksheet   [NUM] = { 0.2, 0.2 };  //Specular Scatter fraction
   G4double CBS_blacksheet   [NUM] = { 0.2, 0.2 };  //backscatter fraction
   G4double BSRFF = 1.0;   
   G4double REFLECTIVITY_blacksheet[NUMENTRIES_water] =
     { 0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF,
       0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF,
       0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF,
       0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF,
       0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF,
       0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF,
       0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF,
       0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 0.057*BSRFF,
       0.059*BSRFF, 0.060*BSRFF, 0.059*BSRFF, 0.058*BSRFF,
       0.057*BSRFF, 0.055*BSRFF, 0.050*BSRFF, 0.045*BSRFF,
       0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF,
       0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF,
       0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF,
       0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF,
       0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF };
   
   G4MaterialPropertiesTable *myST1 = new G4MaterialPropertiesTable();
   myST1->AddProperty("RINDEX",                ENERGY_blacksheet, RINDEX_blacksheet, NUM );
   myST1->AddProperty("SPECULARLOBECONSTANT",  ENERGY_blacksheet, CSL_blacksheet,    NUM);
   myST1->AddProperty("SPECULARSPIKECONSTANT", ENERGY_blacksheet, CSS_blacksheet,    NUM);
   myST1->AddProperty("BACKSCATTERCONSTANT",   ENERGY_blacksheet, CBS_blacksheet,    NUM);
   myST1->AddProperty("REFLECTIVITY",          ENERGY_water,      REFLECTIVITY_blacksheet, NUMENTRIES_water);
   fSurfaces[ "blacksheet" ]->SetMaterialPropertiesTable(myST1);

   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** pmtglass *************************************************************************************
   const unsigned NUMENTRIES_cathode=21;
   G4double ENERGY_cathode[NUMENTRIES_cathode];
   G4double RINDEX_cathode[NUMENTRIES_cathode];
   G4double ABSLENGTH_cathode[NUMENTRIES_cathode];
   G4double lambda_cathode[NUMENTRIES_cathode] = {
     380,      395 ,     410 ,     425 ,     440 ,
     455 ,     470 ,     485 ,     500 ,     515 ,
     530 ,     545 ,     560 ,     575 ,     590 ,
     605 ,     620 ,     635 ,     650 ,     665 ,     680 };
   G4double n_cathode[NUMENTRIES_cathode] = {
     1.92,     2.18,     2.38,     2.61,     2.70,
     2.87,     3.00,     3.00,     3.00,     3.09,
     3.26,     3.20,     3.12,     3.06,     3.01,
     2.98,     2.96,     2.95,     2.95,     2.95,     2.96 };
   G4double k_cathode[NUMENTRIES_cathode] = {
     1.69, 1.69, 1.71, 1.53, 1.50,
     1.44, 1.34, 1.11, 1.06, 1.05,
     0.86, 0.63, 0.53, 0.46, 0.42,
     0.38, 0.37, 0.35, 0.34, 0.34, 0.33 };
   // convert k to absorption length a:
   // a = lambda / ( 4*pi*k )
   double scale_abs_length = 1000.0/30.0;
   for ( unsigned i=0; i<NUMENTRIES_cathode; ++i ){
     ENERGY_cathode[NUMENTRIES_cathode-i-1] = 1239.8 / lambda_cathode[i] * CLHEP::eV;
     RINDEX_cathode[NUMENTRIES_cathode-i-1] = n_cathode[i];
     // scale the abs. length.  expect cathode is really only 30 nm thick, but, we make it 1 um thick in geometry
     // so lets scale abs. length to be 1000/30 longer than expected
     ABSLENGTH_cathode[NUMENTRIES_cathode-i-1] = lambda_cathode[i] / ( 4.0*CLHEP::pi*k_cathode[i] ) * scale_abs_length * CLHEP::nm; 
   }   
   G4MaterialPropertiesTable *myMPTcath = new G4MaterialPropertiesTable();
   myMPTcath->AddProperty("RINDEX",   ENERGY_cathode, RINDEX_cathode,     NUMENTRIES_cathode);
   myMPTcath->AddProperty("ABSLENGTH",ENERGY_cathode, ABSLENGTH_cathode,  NUMENTRIES_cathode);
   fSurfaces[ "cathode" ] = new G4OpticalSurface("cathode" );
   fSurfaces[ "cathode" ]->SetType(dielectric_dielectric);
   fSurfaces[ "cathode" ]->SetModel(unified);  
   fSurfaces[ "cathode" ]->SetFinish(polished);
   fSurfaces[ "cathode" ]->SetSigmaAlpha( 0.013 );
   fSurfaces[ "cathode" ]->SetMaterialPropertiesTable(myMPTcath);

   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** fiber reinforced plastic  ********************************************************************
   fSurfaces[ "frp" ] = new G4OpticalSurface("PmtGlassSurface");
   fSurfaces[ "frp" ]->SetType(dielectric_dielectric);
   fSurfaces[ "frp" ]->SetModel(unified);  
   fSurfaces[ "frp" ]->SetFinish(groundfrontpainted);
   G4double ENERGY_frp[NUM]  = { 1.4*CLHEP::eV, 6.2*CLHEP::eV };
   G4double REFLECT_frp[NUM] = { 0.5, 0.5 };
   G4MaterialPropertiesTable *frpMPT = new G4MaterialPropertiesTable();
   frpMPT->AddProperty("REFLECTIVITY",          ENERGY_frp, REFLECT_frp, NUM);
   fSurfaces[ "frp" ]->SetMaterialPropertiesTable(frpMPT);

   
   
   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** pmtglass *************************************************************************************
   fSurfaces[ "pmtglass" ] = new G4OpticalSurface("PmtGlassSurface");
   fSurfaces[ "pmtglass" ]->SetType(dielectric_dielectric);
   fSurfaces[ "pmtglass" ]->SetModel(unified);  
   fSurfaces[ "pmtglass" ]->SetFinish(ground);
   fSurfaces[ "pmtglass" ]->SetSigmaAlpha( 0.013 );
   G4double RGCFF = 0.32;
   G4double ENERGY_glass[NUM] = { 1.4*CLHEP::eV, 6.2*CLHEP::eV };
   G4double CSL_glass   [NUM] = { 0.98, 0.98 };  //Specular Lobe fraction
   G4double CSS_glass   [NUM] = { 0.02, 0.02 };  //Specular Scatter fraction
   G4double CBS_glass   [NUM] = { 0.0, 0.0 };  //backscatter fraction

   
   G4double REFLECTIVITY_glass[NUM] = { 1.0*RGCFF, 1.0*RGCFF };
   G4double ABSORPTION_glass[NUMENTRIES_water]= 
     { 1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm };
   G4MaterialPropertiesTable *myST2 = new G4MaterialPropertiesTable();
   myST2->AddProperty("SPECULARLOBECONSTANT",  ENERGY_glass, CSL_glass, NUM);
   myST2->AddProperty("SPECULARSPIKECONSTANT", ENERGY_glass, CSS_glass, NUM);
   myST2->AddProperty("BACKSCATTERCONSTANT",   ENERGY_glass, CBS_glass, NUM);
   myST2->AddProperty("REFLECTIVITY",          ENERGY_water, REFLECTIVITY_glass, NUM);
   myST2->AddProperty("ABSLENGTH",             ENERGY_water, ABSORPTION_glass, NUM);
   fSurfaces[ "pmtglass" ]->SetMaterialPropertiesTable(myST2);

   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** white tyvek *************************************************************************
   fSurfaces[ "tyvek" ] = new G4OpticalSurface("TyvekSurface");
   fSurfaces[ "tyvek" ]->SetType( dielectric_LUTDAVIS );
   fSurfaces[ "tyvek" ]->SetModel( DAVIS );
   fSurfaces[ "tyvek" ]->SetFinish( polishedteflonair );

   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** reflector *************************************************************************
   fSurfaces[ "reflector" ] = new G4OpticalSurface("reflector");
   fSurfaces[ "reflector" ]->SetType( dielectric_metal );
   fSurfaces[ "reflector" ]->SetModel( unified );
   fSurfaces[ "reflector" ]->SetFinish( polished );
   G4double ENE_refl[NUM] = {1.0*CLHEP::eV, 6.0*CLHEP::eV};
   G4double REF_refl[NUM] = {0.95, 0.95};
   G4MaterialPropertiesTable *mySTrefl = new G4MaterialPropertiesTable();
   mySTrefl->AddProperty("REFLECTIVITY", ENE_refl, REF_refl, NUM);
   fSurfaces[ "reflector" ]->SetMaterialPropertiesTable( mySTrefl );

   
   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** pmtsteel *************************************************************************
   fSurfaces[ "pmtsteel" ] = new G4OpticalSurface("pmtsteel");
   fSurfaces[ "pmtsteel" ]->SetType(dielectric_metal);
   fSurfaces[ "pmtsteel" ]->SetModel(unified); 
   fSurfaces[ "pmtsteel" ]->SetFinish(polished);
   G4double ENE_pmtsteel[NUM] = {1.0*CLHEP::eV, 6.0*CLHEP::eV};
   G4double REF_pmtsteel[NUM] = {0.8, 0.8};
   G4MaterialPropertiesTable *mySTeel = new G4MaterialPropertiesTable();
   mySTeel->AddProperty("REFLECTIVITY", ENE_pmtsteel, REF_pmtsteel, NUM);
   fSurfaces[ "pmtsteel" ]->SetMaterialPropertiesTable( mySTeel );
   
   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** acrylic *************************************************************************
   fSurfaces[ "acrylic" ] = new G4OpticalSurface("AcrylicSurface");
   fSurfaces[ "acrylic" ]->SetType(dielectric_dielectric);
   fSurfaces[ "acrylic" ]->SetModel(unified);  
   fSurfaces[ "acrylic" ]->SetFinish(ground);
   fSurfaces[ "acrylic" ]->SetSigmaAlpha( 0.02 );
   G4double ENERGY_skAcrylic[NUMENTRIES_acrylic] =
     { 2.066*CLHEP::eV, 2.070*CLHEP::eV, 2.073*CLHEP::eV, 2.077*CLHEP::eV, 2.080*CLHEP::eV, 2.084*CLHEP::eV, 2.087*CLHEP::eV,
       2.091*CLHEP::eV, 2.094*CLHEP::eV, 2.098*CLHEP::eV, 2.101*CLHEP::eV, 2.105*CLHEP::eV, 2.109*CLHEP::eV, 2.112*CLHEP::eV,
       2.116*CLHEP::eV, 2.119*CLHEP::eV, 2.123*CLHEP::eV, 2.127*CLHEP::eV, 2.130*CLHEP::eV, 2.134*CLHEP::eV, 2.138*CLHEP::eV,
       2.141*CLHEP::eV, 2.145*CLHEP::eV, 2.149*CLHEP::eV, 2.153*CLHEP::eV, 2.156*CLHEP::eV, 2.160*CLHEP::eV, 2.164*CLHEP::eV,
       2.168*CLHEP::eV, 2.171*CLHEP::eV, 2.175*CLHEP::eV, 2.179*CLHEP::eV, 2.183*CLHEP::eV, 2.187*CLHEP::eV, 2.191*CLHEP::eV,
       2.194*CLHEP::eV, 2.198*CLHEP::eV, 2.202*CLHEP::eV, 2.206*CLHEP::eV, 2.210*CLHEP::eV, 2.214*CLHEP::eV, 2.218*CLHEP::eV,
       2.222*CLHEP::eV, 2.226*CLHEP::eV, 2.230*CLHEP::eV, 2.234*CLHEP::eV, 2.238*CLHEP::eV, 2.242*CLHEP::eV, 2.246*CLHEP::eV,
       2.250*CLHEP::eV, 2.254*CLHEP::eV, 2.258*CLHEP::eV, 2.262*CLHEP::eV, 2.267*CLHEP::eV, 2.271*CLHEP::eV, 2.275*CLHEP::eV,
       2.279*CLHEP::eV, 2.283*CLHEP::eV, 2.288*CLHEP::eV, 2.292*CLHEP::eV, 2.296*CLHEP::eV, 2.300*CLHEP::eV, 2.305*CLHEP::eV,
       2.309*CLHEP::eV, 2.313*CLHEP::eV, 2.317*CLHEP::eV, 2.322*CLHEP::eV, 2.326*CLHEP::eV, 2.331*CLHEP::eV, 2.335*CLHEP::eV,
       2.339*CLHEP::eV, 2.344*CLHEP::eV, 2.348*CLHEP::eV, 2.353*CLHEP::eV, 2.357*CLHEP::eV, 2.362*CLHEP::eV, 2.366*CLHEP::eV,
       2.371*CLHEP::eV, 2.375*CLHEP::eV, 2.380*CLHEP::eV, 2.384*CLHEP::eV, 2.389*CLHEP::eV, 2.394*CLHEP::eV, 2.398*CLHEP::eV,
       2.403*CLHEP::eV, 2.407*CLHEP::eV, 2.412*CLHEP::eV, 2.417*CLHEP::eV, 2.422*CLHEP::eV, 2.426*CLHEP::eV, 2.431*CLHEP::eV,
       2.436*CLHEP::eV, 2.441*CLHEP::eV, 2.445*CLHEP::eV, 2.450*CLHEP::eV, 2.455*CLHEP::eV, 2.460*CLHEP::eV, 2.465*CLHEP::eV,
       2.470*CLHEP::eV, 2.475*CLHEP::eV, 2.480*CLHEP::eV, 2.485*CLHEP::eV, 2.490*CLHEP::eV, 2.495*CLHEP::eV, 2.500*CLHEP::eV,
       2.505*CLHEP::eV, 2.510*CLHEP::eV, 2.515*CLHEP::eV, 2.520*CLHEP::eV, 2.525*CLHEP::eV, 2.530*CLHEP::eV, 2.535*CLHEP::eV,
       2.541*CLHEP::eV, 2.546*CLHEP::eV, 2.551*CLHEP::eV, 2.556*CLHEP::eV, 2.562*CLHEP::eV, 2.567*CLHEP::eV, 2.572*CLHEP::eV,
       2.578*CLHEP::eV, 2.583*CLHEP::eV, 2.588*CLHEP::eV, 2.594*CLHEP::eV, 2.599*CLHEP::eV, 2.605*CLHEP::eV, 2.610*CLHEP::eV,
       2.616*CLHEP::eV, 2.621*CLHEP::eV, 2.627*CLHEP::eV, 2.632*CLHEP::eV, 2.638*CLHEP::eV, 2.644*CLHEP::eV, 2.649*CLHEP::eV,
       2.655*CLHEP::eV, 2.661*CLHEP::eV, 2.666*CLHEP::eV, 2.672*CLHEP::eV, 2.678*CLHEP::eV, 2.684*CLHEP::eV, 2.689*CLHEP::eV,
       2.695*CLHEP::eV, 2.701*CLHEP::eV, 2.707*CLHEP::eV, 2.713*CLHEP::eV, 2.719*CLHEP::eV, 2.725*CLHEP::eV, 2.731*CLHEP::eV,
       2.737*CLHEP::eV, 2.743*CLHEP::eV, 2.749*CLHEP::eV, 2.755*CLHEP::eV, 2.761*CLHEP::eV, 2.768*CLHEP::eV, 2.774*CLHEP::eV,
       2.780*CLHEP::eV, 2.786*CLHEP::eV, 2.792*CLHEP::eV, 2.799*CLHEP::eV, 2.805*CLHEP::eV, 2.811*CLHEP::eV, 2.818*CLHEP::eV,
       2.824*CLHEP::eV, 2.831*CLHEP::eV, 2.837*CLHEP::eV, 2.844*CLHEP::eV, 2.850*CLHEP::eV, 2.857*CLHEP::eV, 2.863*CLHEP::eV,
       2.870*CLHEP::eV, 2.877*CLHEP::eV, 2.883*CLHEP::eV, 2.890*CLHEP::eV, 2.897*CLHEP::eV, 2.904*CLHEP::eV, 2.910*CLHEP::eV,
       2.917*CLHEP::eV, 2.924*CLHEP::eV, 2.931*CLHEP::eV, 2.938*CLHEP::eV, 2.945*CLHEP::eV, 2.952*CLHEP::eV, 2.959*CLHEP::eV,
       2.966*CLHEP::eV, 2.973*CLHEP::eV, 2.980*CLHEP::eV, 2.988*CLHEP::eV, 2.995*CLHEP::eV, 3.002*CLHEP::eV, 3.009*CLHEP::eV,
       3.017*CLHEP::eV, 3.024*CLHEP::eV, 3.031*CLHEP::eV, 3.039*CLHEP::eV, 3.046*CLHEP::eV, 3.054*CLHEP::eV, 3.061*CLHEP::eV,
       3.069*CLHEP::eV, 3.077*CLHEP::eV, 3.084*CLHEP::eV, 3.092*CLHEP::eV, 3.100*CLHEP::eV, 3.107*CLHEP::eV, 3.115*CLHEP::eV,
       3.123*CLHEP::eV, 3.131*CLHEP::eV, 3.139*CLHEP::eV, 3.147*CLHEP::eV, 3.155*CLHEP::eV, 3.163*CLHEP::eV, 3.171*CLHEP::eV,
       3.179*CLHEP::eV, 3.187*CLHEP::eV, 3.195*CLHEP::eV, 3.204*CLHEP::eV, 3.212*CLHEP::eV, 3.220*CLHEP::eV, 3.229*CLHEP::eV,
       3.237*CLHEP::eV, 3.246*CLHEP::eV, 3.254*CLHEP::eV, 3.263*CLHEP::eV, 3.271*CLHEP::eV, 3.280*CLHEP::eV, 3.289*CLHEP::eV,
       3.297*CLHEP::eV, 3.306*CLHEP::eV, 3.315*CLHEP::eV, 3.324*CLHEP::eV, 3.333*CLHEP::eV, 3.342*CLHEP::eV, 3.351*CLHEP::eV, 
       3.360*CLHEP::eV, 3.369*CLHEP::eV, 3.378*CLHEP::eV, 3.388*CLHEP::eV, 3.397*CLHEP::eV, 3.406*CLHEP::eV, 3.416*CLHEP::eV, 
       3.425*CLHEP::eV, 3.434*CLHEP::eV, 3.444*CLHEP::eV, 3.454*CLHEP::eV, 3.463*CLHEP::eV, 3.473*CLHEP::eV, 3.483*CLHEP::eV, 
       3.493*CLHEP::eV, 3.502*CLHEP::eV, 3.512*CLHEP::eV, 3.522*CLHEP::eV, 3.532*CLHEP::eV, 3.542*CLHEP::eV, 3.553*CLHEP::eV, 
       3.563*CLHEP::eV, 3.573*CLHEP::eV, 3.583*CLHEP::eV, 3.594*CLHEP::eV, 3.604*CLHEP::eV, 3.615*CLHEP::eV, 3.625*CLHEP::eV, 
       3.636*CLHEP::eV, 3.647*CLHEP::eV, 3.657*CLHEP::eV, 3.668*CLHEP::eV, 3.679*CLHEP::eV, 3.690*CLHEP::eV, 3.701*CLHEP::eV, 
       3.712*CLHEP::eV, 3.723*CLHEP::eV, 3.734*CLHEP::eV, 3.746*CLHEP::eV, 3.757*CLHEP::eV, 3.769*CLHEP::eV, 3.780*CLHEP::eV, 
       3.792*CLHEP::eV, 3.803*CLHEP::eV, 3.815*CLHEP::eV, 3.827*CLHEP::eV, 3.839*CLHEP::eV, 3.850*CLHEP::eV, 3.862*CLHEP::eV, 
       3.875*CLHEP::eV, 3.887*CLHEP::eV, 3.899*CLHEP::eV, 3.911*CLHEP::eV, 3.924*CLHEP::eV, 3.936*CLHEP::eV, 3.949*CLHEP::eV, 
       3.961*CLHEP::eV, 3.974*CLHEP::eV, 3.987*CLHEP::eV, 3.999*CLHEP::eV, 4.012*CLHEP::eV, 4.025*CLHEP::eV, 4.039*CLHEP::eV, 
       4.052*CLHEP::eV, 4.065*CLHEP::eV, 4.078*CLHEP::eV, 4.092*CLHEP::eV, 4.105*CLHEP::eV, 4.119*CLHEP::eV, 4.133*CLHEP::eV, 
       4.147*CLHEP::eV, 4.161*CLHEP::eV, 4.175*CLHEP::eV, 4.189*CLHEP::eV, 4.203*CLHEP::eV };

   // TF: from http://refractiveindex.info/?shelf=organic&book=poly%28methyl_methacrylate%29&page=Szczurowski
   G4double RINDEX_skAcrylic[NUMENTRIES_acrylic] =
     {1.4901, 1.4901, 1.4902, 1.4902, 1.4902, 1.4903, 1.4903, 1.4904, 1.4904, 1.4904, 
      1.4905, 1.4905, 1.4906, 1.4906, 1.4907, 1.4907, 1.4908, 1.4908, 1.4908, 1.4909, 
      1.4909, 1.4910, 1.4910, 1.4911, 1.4911, 1.4912, 1.4912, 1.4913, 1.4913, 1.4913, 
      1.4914, 1.4914, 1.4915, 1.4915, 1.4916, 1.4916, 1.4917, 1.4917, 1.4918, 1.4918, 
      1.4919, 1.4919, 1.4920, 1.4920, 1.4921, 1.4921, 1.4922, 1.4922, 1.4923, 1.4923, 
      1.4924, 1.4924, 1.4925, 1.4926, 1.4926, 1.4927, 1.4927, 1.4928, 1.4928, 1.4929, 
      1.4929, 1.4930, 1.4931, 1.4931, 1.4932, 1.4932, 1.4933, 1.4933, 1.4934, 1.4935, 
      1.4935, 1.4936, 1.4936, 1.4937, 1.4938, 1.4938, 1.4939, 1.4940, 1.4940, 1.4941, 
      1.4941, 1.4942, 1.4943, 1.4943, 1.4944, 1.4945, 1.4945, 1.4946, 1.4947, 1.4947, 
      1.4948, 1.4949, 1.4949, 1.4950, 1.4951, 1.4951, 1.4952, 1.4953, 1.4954, 1.4954, 
      1.4955, 1.4956, 1.4956, 1.4957, 1.4958, 1.4959, 1.4959, 1.4960, 1.4961, 1.4962, 
      1.4962, 1.4963, 1.4964, 1.4965, 1.4965, 1.4966, 1.4967, 1.4968, 1.4969, 1.4970, 
      1.4970, 1.4971, 1.4972, 1.4973, 1.4974, 1.4974, 1.4975, 1.4976, 1.4977, 1.4978, 
      1.4979, 1.4980, 1.4980, 1.4981, 1.4982, 1.4983, 1.4984, 1.4985, 1.4986, 1.4987, 
      1.4988, 1.4989, 1.4990, 1.4991, 1.4992, 1.4993, 1.4994, 1.4995, 1.4996, 1.4997, 
      1.4997, 1.4998, 1.5000, 1.5001, 1.5002, 1.5003, 1.5004, 1.5005, 1.5006, 1.5007, 
      1.5008, 1.5009, 1.5010, 1.5011, 1.5012, 1.5013, 1.5015, 1.5016, 1.5017, 1.5018, 
      1.5019, 1.5020, 1.5022, 1.5023, 1.5024, 1.5025, 1.5026, 1.5028, 1.5029, 1.5030, 
      1.5031, 1.5033, 1.5034, 1.5035, 1.5036, 1.5038, 1.5039, 1.5040, 1.5042, 1.5043, 
      1.5044, 1.5046, 1.5047, 1.5049, 1.5050, 1.5051, 1.5053, 1.5054, 1.5056, 1.5057, 
      1.5059, 1.5060, 1.5062, 1.5063, 1.5065, 1.5066, 1.5068, 1.5069, 1.5071, 1.5073, 
      1.5074, 1.5076, 1.5077, 1.5079, 1.5081, 1.5082, 1.5084, 1.5086, 1.5088, 1.5089, 
      1.5091, 1.5093, 1.5095, 1.5096, 1.5098, 1.5100, 1.5102, 1.5104, 1.5106, 1.5108, 
      1.5109, 1.5111, 1.5113, 1.5115, 1.5117, 1.5119, 1.5121, 1.5123, 1.5125, 1.5127, 
      1.5130, 1.5132, 1.5134, 1.5136, 1.5138, 1.5141, 1.5143, 1.5145, 1.5147, 1.5149, 
      1.5152, 1.5154, 1.5157, 1.5159, 1.5161, 1.5164, 1.5166, 1.5169, 1.5171, 1.5174, 
      1.5176, 1.5179, 1.5181, 1.5184, 1.5187, 1.5190, 1.5192, 1.5195, 1.5198, 1.5201, 
      1.5203, 1.5206, 1.5209, 1.5212, 1.5215, 1.5218, 1.5221, 1.5224, 1.5227, 1.5230, 
      1.5234, 1.5237, 1.5240, 1.5243, 1.5247, 1.5250, 1.5254, 1.5257, 1.5260, 1.5264, 
      1.5267, 1.5271, 1.5275, 1.5279, 1.5282, 1.5286, 1.5290, 1.5294, 1.5297, 1.5302, 
      1.5306, 1.5310, 1.5314, 1.5318, 1.5322, 1.5326};

   // Table from acrsg.F is abs coeff, so converted to abs length in m and inverted as
   // it was original a function of wavelength and now photon energy in increased energies.
   G4double ABSORPTION_skAcrylic[NUMENTRIES_acrylic] =
     {3472.22*CLHEP::m, 3355.70*CLHEP::m, 3300.33*CLHEP::m, 3412.97*CLHEP::m, 3496.50*CLHEP::m, 3649.64*CLHEP::m, 3861.00*CLHEP::m, 3921.57*CLHEP::m, 3846.15*CLHEP::m, 3703.70*CLHEP::m, 
      3448.28*CLHEP::m, 3048.78*CLHEP::m, 2932.55*CLHEP::m, 2770.08*CLHEP::m, 2808.99*CLHEP::m, 2906.98*CLHEP::m, 2898.55*CLHEP::m, 2906.98*CLHEP::m, 3003.00*CLHEP::m, 3164.56*CLHEP::m, 
      3184.71*CLHEP::m, 3246.75*CLHEP::m, 3086.42*CLHEP::m, 2994.01*CLHEP::m, 2994.01*CLHEP::m, 2967.36*CLHEP::m, 3030.30*CLHEP::m, 3039.51*CLHEP::m, 3105.59*CLHEP::m, 2958.58*CLHEP::m, 
      3012.05*CLHEP::m, 3039.51*CLHEP::m, 2967.36*CLHEP::m, 2932.55*CLHEP::m, 2923.98*CLHEP::m, 2890.17*CLHEP::m, 2857.14*CLHEP::m, 2865.33*CLHEP::m, 2915.45*CLHEP::m, 2898.55*CLHEP::m, 
      2881.84*CLHEP::m, 2754.82*CLHEP::m, 2702.70*CLHEP::m, 2754.82*CLHEP::m, 2890.17*CLHEP::m, 2840.91*CLHEP::m, 2659.57*CLHEP::m, 2604.17*CLHEP::m, 2624.67*CLHEP::m, 2857.14*CLHEP::m, 
      2849.00*CLHEP::m, 2849.00*CLHEP::m, 2610.97*CLHEP::m, 2597.40*CLHEP::m, 2688.17*CLHEP::m, 2590.67*CLHEP::m, 2439.02*CLHEP::m, 2380.95*CLHEP::m, 2398.08*CLHEP::m, 2320.19*CLHEP::m, 
      2369.67*CLHEP::m, 2293.58*CLHEP::m, 2267.57*CLHEP::m, 2262.44*CLHEP::m, 2320.19*CLHEP::m, 2309.47*CLHEP::m, 2237.14*CLHEP::m, 2247.19*CLHEP::m, 2257.34*CLHEP::m, 2336.45*CLHEP::m, 
      2347.42*CLHEP::m, 2421.31*CLHEP::m, 2439.02*CLHEP::m, 2392.34*CLHEP::m, 2392.34*CLHEP::m, 2500.00*CLHEP::m, 2500.00*CLHEP::m, 2512.56*CLHEP::m, 2481.39*CLHEP::m, 2444.99*CLHEP::m, 
      2155.17*CLHEP::m, 2053.39*CLHEP::m, 2008.03*CLHEP::m, 2040.82*CLHEP::m, 1988.07*CLHEP::m, 2004.01*CLHEP::m, 1976.28*CLHEP::m, 1834.86*CLHEP::m, 1798.56*CLHEP::m, 1869.16*CLHEP::m, 
      1808.32*CLHEP::m, 1754.39*CLHEP::m, 1805.05*CLHEP::m, 1841.62*CLHEP::m, 1872.66*CLHEP::m, 1964.64*CLHEP::m, 1976.28*CLHEP::m, 1893.94*CLHEP::m, 1890.36*CLHEP::m, 1908.40*CLHEP::m, 
      1897.53*CLHEP::m, 1855.29*CLHEP::m, 1886.79*CLHEP::m, 1904.76*CLHEP::m, 1841.62*CLHEP::m, 1855.29*CLHEP::m, 1834.86*CLHEP::m, 1805.05*CLHEP::m, 1733.10*CLHEP::m, 1779.36*CLHEP::m, 
      1754.39*CLHEP::m, 1788.91*CLHEP::m, 1831.50*CLHEP::m, 1834.86*CLHEP::m, 1869.16*CLHEP::m, 1912.05*CLHEP::m, 2004.01*CLHEP::m, 1968.50*CLHEP::m, 1915.71*CLHEP::m, 1818.18*CLHEP::m, 
      1795.33*CLHEP::m, 1766.78*CLHEP::m, 1808.32*CLHEP::m, 1766.78*CLHEP::m, 1754.39*CLHEP::m, 1751.31*CLHEP::m, 1776.20*CLHEP::m, 1838.24*CLHEP::m, 1872.66*CLHEP::m, 1919.39*CLHEP::m, 
      1901.14*CLHEP::m, 1988.07*CLHEP::m, 1926.78*CLHEP::m, 1893.94*CLHEP::m, 1901.14*CLHEP::m, 1851.85*CLHEP::m, 1818.18*CLHEP::m, 1769.91*CLHEP::m, 1727.12*CLHEP::m, 1724.14*CLHEP::m, 
      1709.40*CLHEP::m, 1689.19*CLHEP::m, 1692.05*CLHEP::m, 1680.67*CLHEP::m, 1697.79*CLHEP::m, 1727.12*CLHEP::m, 1763.67*CLHEP::m, 1697.79*CLHEP::m, 1692.05*CLHEP::m, 1666.67*CLHEP::m, 
      1600.00*CLHEP::m, 1636.66*CLHEP::m, 1615.51*CLHEP::m, 1610.31*CLHEP::m, 1538.46*CLHEP::m, 1501.50*CLHEP::m, 1519.76*CLHEP::m, 1492.54*CLHEP::m, 1515.15*CLHEP::m, 1457.73*CLHEP::m, 
      1479.29*CLHEP::m, 1461.99*CLHEP::m, 1470.59*CLHEP::m, 1470.59*CLHEP::m, 1453.49*CLHEP::m, 1512.86*CLHEP::m, 1522.07*CLHEP::m, 1589.83*CLHEP::m, 1577.29*CLHEP::m, 1597.44*CLHEP::m, 
      1600.00*CLHEP::m, 1672.24*CLHEP::m, 1663.89*CLHEP::m, 1647.45*CLHEP::m, 1661.13*CLHEP::m, 1607.72*CLHEP::m, 1543.21*CLHEP::m, 1488.10*CLHEP::m, 1459.85*CLHEP::m, 1373.63*CLHEP::m, 
      1326.26*CLHEP::m, 1373.63*CLHEP::m, 1410.44*CLHEP::m, 1406.47*CLHEP::m, 1396.65*CLHEP::m, 1308.90*CLHEP::m, 1328.02*CLHEP::m, 1386.96*CLHEP::m, 1347.71*CLHEP::m, 1297.02*CLHEP::m, 
      1262.63*CLHEP::m, 1234.57*CLHEP::m, 1283.70*CLHEP::m, 1319.26*CLHEP::m, 1310.62*CLHEP::m, 1319.26*CLHEP::m, 1402.52*CLHEP::m, 1459.85*CLHEP::m, 1464.13*CLHEP::m, 1472.75*CLHEP::m, 
      1420.45*CLHEP::m, 1432.66*CLHEP::m, 1400.56*CLHEP::m, 1362.40*CLHEP::m, 1358.70*CLHEP::m, 1358.70*CLHEP::m, 1379.31*CLHEP::m, 1360.54*CLHEP::m, 1362.40*CLHEP::m, 1355.01*CLHEP::m, 
      1342.28*CLHEP::m, 1367.99*CLHEP::m, 1293.66*CLHEP::m, 1221.00*CLHEP::m, 1197.60*CLHEP::m, 1165.50*CLHEP::m, 1119.82*CLHEP::m, 1107.42*CLHEP::m, 1108.65*CLHEP::m, 1040.58*CLHEP::m, 
      1033.06*CLHEP::m, 1000.00*CLHEP::m,  952.38*CLHEP::m,  925.93*CLHEP::m,  909.09*CLHEP::m,  869.57*CLHEP::m,  800.00*CLHEP::m,  781.25*CLHEP::m,  769.23*CLHEP::m,  729.93*CLHEP::m,
      735.29*CLHEP::m,   729.93*CLHEP::m,  709.22*CLHEP::m,  684.93*CLHEP::m,  657.89*CLHEP::m,  645.16*CLHEP::m,  625.00*CLHEP::m,  581.40*CLHEP::m,  568.18*CLHEP::m,  558.66*CLHEP::m,
      543.48*CLHEP::m,   540.54*CLHEP::m,  523.56*CLHEP::m,  505.05*CLHEP::m,  490.20*CLHEP::m,  485.44*CLHEP::m,  462.96*CLHEP::m,  450.45*CLHEP::m,  442.48*CLHEP::m,  429.18*CLHEP::m,
      420.17*CLHEP::m,   411.52*CLHEP::m,  411.52*CLHEP::m,  393.70*CLHEP::m,  393.70*CLHEP::m,  386.10*CLHEP::m,  381.68*CLHEP::m,  355.87*CLHEP::m,  343.64*CLHEP::m,  331.13*CLHEP::m,
      343.64*CLHEP::m,   338.98*CLHEP::m,  335.57*CLHEP::m,  326.80*CLHEP::m,  313.48*CLHEP::m,  303.95*CLHEP::m,  294.12*CLHEP::m,  292.40*CLHEP::m,  280.90*CLHEP::m,  273.97*CLHEP::m,
      270.27*CLHEP::m,   265.25*CLHEP::m,  261.78*CLHEP::m,  256.41*CLHEP::m,  250.00*CLHEP::m,  242.13*CLHEP::m,  237.53*CLHEP::m,  233.10*CLHEP::m,  228.83*CLHEP::m,  226.76*CLHEP::m,
      223.21*CLHEP::m,   219.30*CLHEP::m,  215.05*CLHEP::m,  211.86*CLHEP::m,  208.77*CLHEP::m,  206.61*CLHEP::m,  201.21*CLHEP::m,  196.85*CLHEP::m,  193.42*CLHEP::m,  189.75*CLHEP::m,
      186.22*CLHEP::m,   182.15*CLHEP::m,  175.75*CLHEP::m,  168.92*CLHEP::m,  162.60*CLHEP::m,  154.80*CLHEP::m,  144.51*CLHEP::m,  133.51*CLHEP::m,  120.19*CLHEP::m,  105.93*CLHEP::m,
      90.91*CLHEP::m,     75.76*CLHEP::m,   61.35*CLHEP::m,   48.54*CLHEP::m,   37.88*CLHEP::m,   29.41*CLHEP::m };

   G4double ENERGY_acrylic[NUM] = { 2.066*CLHEP::eV, 4.203*CLHEP::eV };
   G4double CSL_acrylic   [NUM] = { 0.20, 0.20 };  //Specular Lobe fraction
   G4double CSS_acrylic   [NUM] = { 0.70, 0.70 };  //Specular Scatter fraction
   G4double CBS_acrylic   [NUM] = { 0.02, 0.02 };  //backscatter fraction

   G4MaterialPropertiesTable *AcrPropTable = new G4MaterialPropertiesTable();
   AcrPropTable->AddProperty("RINDEX",                ENERGY_skAcrylic, RINDEX_skAcrylic,     NUMENTRIES_acrylic);
   AcrPropTable->AddProperty("ABSLENGTH",             ENERGY_skAcrylic, ABSORPTION_skAcrylic, NUMENTRIES_acrylic);
   AcrPropTable->AddProperty("SPECULARLOBECONSTANT",  ENERGY_acrylic,   CSL_acrylic, NUM);
   AcrPropTable->AddProperty("SPECULARSPIKECONSTANT", ENERGY_acrylic,   CSS_acrylic, NUM);
   AcrPropTable->AddProperty("BACKSCATTERCONSTANT",   ENERGY_acrylic,   CBS_acrylic, NUM);
   fSurfaces[ "acrylic" ]->SetMaterialPropertiesTable(AcrPropTable);

   
   
}




