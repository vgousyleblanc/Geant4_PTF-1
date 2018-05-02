#include "MaterialsMap.hh"
#include "G4NistManager.hh"

MaterialsMap * MaterialsMap::fMaterialsMap = nullptr;
std::map< std::string, G4Material * > MaterialsMap::fMaterials;

MaterialsMap& MaterialsMap::Get() {
  std::cout<<"MaterialsMap::Get()"<<std::endl;
  if ( fMaterialsMap==nullptr ) MaterialsMap();
  return *fMaterialsMap;
}

G4Material* MaterialsMap::operator[]( const std::string& key ) const{
  if ( fMaterials[key] == nullptr ) throw Invalid();
  return fMaterials[ key ];
}

void MaterialsMap::get_material_names( std::vector< std::string > & matnames ) const{
  for ( const auto & curpair : fMaterials ){
    matnames.push_back( curpair.first );
  }
}

MaterialsMap::MaterialsMap(){
  std::cout<<"MaterialsMap constructor"<<std::endl;
  if ( fMaterialsMap == nullptr ){
    BuildMaterials();
    fMaterialsMap = this;
    std::cout << *fMaterialsMap;
  }
}


void MaterialsMap::BuildMaterials(){
  std::cout<<"MaterialsMap::BuildMaterials"<<std::endl;
  // pmtglass, pmtsteel, cathode, vacuum, acrylic
  G4NistManager* nist = G4NistManager::Instance();

  fMaterials[ "vacuum" ] = nist->FindOrBuildMaterial( "G4_Galactic" );
  fMaterials[ "acrylic" ] = nist->FindOrBuildMaterial( "G4_PLEXIGLASS" );

  G4double a= 12.01*CLHEP::g/CLHEP::mole;
  G4Element* elC = new G4Element("Carbon","C", 6,a);
  a = 1.01*CLHEP::g/ CLHEP::mole;
  G4Element* elH = new G4Element("Hydrogen","H", 1,a);

  
 //---Black sheet
  G4double density = 0.95*CLHEP::g/CLHEP::cm3;
  G4Material* Blacksheet
    = new G4Material("Blacksheet",density,2);
  Blacksheet->AddElement(elC, 1);
  Blacksheet->AddElement(elH, 2);
  fMaterials[ "blacksheet" ] = Blacksheet;

  std::cout<<"Finished blacksheet defn"<<std::endl;
  
  //---Tyvek - jl145
  density = 0.38*CLHEP::g/CLHEP::cm3;  //cf. DuPont product handbook
  G4Material* Tyvek = new G4Material("Tyvek",density,2);
  Tyvek->AddElement(elC, 1);  //polyethylene
  Tyvek->AddElement(elH, 2);
  fMaterials[ "tyvek" ] = Tyvek;
  
  std::cout<<"Finished tyvek defn"<<std::endl;

  
  
  fMaterials[ "pmtsteel" ] = nist->FindOrBuildMaterial( "G4_STAINLESS-STEEL" );
  G4MaterialPropertiesTable *mpt = new G4MaterialPropertiesTable();   
  const G4int nEntries = 2;
  G4double photonEnergy[nEntries] = {1.*CLHEP::eV , 7.*CLHEP::eV};
  G4double abslength_Steel[nEntries] = {.001*CLHEP::mm , .001*CLHEP::mm};
  mpt->AddProperty("ABSLENGTH", photonEnergy, abslength_Steel, nEntries);
  fMaterials[ "pmtsteel" ]->SetMaterialPropertiesTable(mpt);
 std::cout<<"Finished pmtsteel defn"<<std::endl;

  
  fMaterials[ "Al" ] = nist->FindOrBuildMaterial( "G4_Al" );
  G4MaterialPropertiesTable *mptal = new G4MaterialPropertiesTable();   
  mptal->AddProperty("ABSLENGTH", photonEnergy, abslength_Steel, nEntries);
  fMaterials[ "Al" ]->SetMaterialPropertiesTable(mptal);
 std::cout<<"Finished Al defn"<<std::endl;
  
  fMaterials[ "water" ] = nist->FindOrBuildMaterial( "G4_WATER" );
  
   //From SKDETSIM water absorption
   const G4int NUMENTRIES_water=60;

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

   // M Fechner : new ; define the water refraction index using refsg.F 
   //from skdetsim using the whole range.   
    G4double RINDEX1[NUMENTRIES_water] = 
     {1.32885, 1.32906, 1.32927, 1.32948, 1.3297, 1.32992, 1.33014, 
      1.33037, 1.3306, 1.33084, 1.33109, 1.33134, 1.3316, 1.33186, 1.33213,
      1.33241, 1.3327, 1.33299, 1.33329, 1.33361, 1.33393, 1.33427, 1.33462,
      1.33498, 1.33536, 1.33576, 1.33617, 1.3366, 1.33705, 1.33753, 1.33803,
      1.33855, 1.33911, 1.3397, 1.34033, 1.341, 1.34172, 1.34248, 1.34331,
      1.34419, 1.34515, 1.3462, 1.34733, 1.34858, 1.34994, 1.35145, 1.35312,
      1.35498, 1.35707, 1.35943, 1.36211, 1.36518, 1.36872, 1.37287, 1.37776,
      1.38362, 1.39074, 1.39956, 1.41075, 1.42535};
   

    G4double ABWFF = 1.0;

    // Get from the tuning parameters
    //ABWFF = WCSimTuningParams->GetAbwff();

    //T. Akiri: Values from Skdetsim 
    G4double ABSORPTION_water[NUMENTRIES_water] =
      {
        16.1419*CLHEP::cm*ABWFF,  18.278*CLHEP::cm*ABWFF, 21.0657*CLHEP::cm*ABWFF, 24.8568*CLHEP::cm*ABWFF, 30.3117*CLHEP::cm*ABWFF, 
	38.8341*CLHEP::cm*ABWFF, 54.0231*CLHEP::cm*ABWFF, 81.2306*CLHEP::cm*ABWFF, 120.909*CLHEP::cm*ABWFF, 160.238*CLHEP::cm*ABWFF, 
	193.771*CLHEP::cm*ABWFF, 215.017*CLHEP::cm*ABWFF, 227.747*CLHEP::cm*ABWFF,  243.85*CLHEP::cm*ABWFF, 294.036*CLHEP::cm*ABWFF, 
	321.647*CLHEP::cm*ABWFF,  342.81*CLHEP::cm*ABWFF, 362.827*CLHEP::cm*ABWFF, 378.041*CLHEP::cm*ABWFF, 449.378*CLHEP::cm*ABWFF,
        739.434*CLHEP::cm*ABWFF, 1114.23*CLHEP::cm*ABWFF, 1435.56*CLHEP::cm*ABWFF, 1611.06*CLHEP::cm*ABWFF, 1764.18*CLHEP::cm*ABWFF, 
	2100.95*CLHEP::cm*ABWFF,  2292.9*CLHEP::cm*ABWFF, 2431.33*CLHEP::cm*ABWFF,  3053.6*CLHEP::cm*ABWFF, 4838.23*CLHEP::cm*ABWFF, 
	6539.65*CLHEP::cm*ABWFF, 7682.63*CLHEP::cm*ABWFF, 9137.28*CLHEP::cm*ABWFF, 12220.9*CLHEP::cm*ABWFF, 15270.7*CLHEP::cm*ABWFF, 
	19051.5*CLHEP::cm*ABWFF, 23671.3*CLHEP::cm*ABWFF, 29191.1*CLHEP::cm*ABWFF, 35567.9*CLHEP::cm*ABWFF,   42583*CLHEP::cm*ABWFF,
        49779.6*CLHEP::cm*ABWFF, 56465.3*CLHEP::cm*ABWFF,   61830*CLHEP::cm*ABWFF, 65174.6*CLHEP::cm*ABWFF, 66143.7*CLHEP::cm*ABWFF,   
	  64820*CLHEP::cm*ABWFF,   61635*CLHEP::cm*ABWFF, 57176.2*CLHEP::cm*ABWFF, 52012.1*CLHEP::cm*ABWFF, 46595.7*CLHEP::cm*ABWFF, 
	41242.1*CLHEP::cm*ABWFF, 36146.3*CLHEP::cm*ABWFF, 31415.4*CLHEP::cm*ABWFF, 27097.8*CLHEP::cm*ABWFF, 23205.7*CLHEP::cm*ABWFF, 
	19730.3*CLHEP::cm*ABWFF, 16651.6*CLHEP::cm*ABWFF, 13943.6*CLHEP::cm*ABWFF, 11578.1*CLHEP::cm*ABWFF, 9526.13*CLHEP::cm*ABWFF
      };

   // M Fechner: Rayleigh scattering -- as of version 4.6.2 of GEANT,
   // one may use one's own Rayleigh scattering lengths (the buffer is no
   // longer overwritten for "water", see 4.6.2 release notes)

   // RAYFF = 1/ARAS, for those of you who know SKdetsim...
   // actually that's not quite right because the scattering models
   // are different; in G4 there is no scattering depolarization
   // std value at SK = 0.6. But Mie scattering is implemented
   // in SKdetsim and not in G4

   
  // april 2005 : reduced reflections, let's increase scattering...
  // sep 09: for the large detector like superK the old values are muc better
   //G4double RAYFF = 1.0/1.65;  //old
   //    G4double RAYFF = 1.0/1.5;  

   G4double RAYFF = 0.625;

   // Get from the tuning parameters
   //RAYFF = WCSimTuningParams->GetRayff();
   //    printf("RAYFF: %f\n",RAYFF);

   //T. Akiri: Values from Skdetsim 
   G4double RAYLEIGH_water[NUMENTRIES_water] = {
      386929*CLHEP::cm*RAYFF,  366249*CLHEP::cm*RAYFF,  346398*CLHEP::cm*RAYFF,  327355*CLHEP::cm*RAYFF,  309097*CLHEP::cm*RAYFF,  
      291603*CLHEP::cm*RAYFF,  274853*CLHEP::cm*RAYFF,  258825*CLHEP::cm*RAYFF,  243500*CLHEP::cm*RAYFF,  228856*CLHEP::cm*RAYFF,  
      214873*CLHEP::cm*RAYFF,  201533*CLHEP::cm*RAYFF,  188816*CLHEP::cm*RAYFF,  176702*CLHEP::cm*RAYFF,  165173*CLHEP::cm*RAYFF,
      154210*CLHEP::cm*RAYFF,  143795*CLHEP::cm*RAYFF,  133910*CLHEP::cm*RAYFF,  124537*CLHEP::cm*RAYFF,  115659*CLHEP::cm*RAYFF,  
      107258*CLHEP::cm*RAYFF, 99318.2*CLHEP::cm*RAYFF, 91822.2*CLHEP::cm*RAYFF,   84754*CLHEP::cm*RAYFF, 78097.3*CLHEP::cm*RAYFF, 
     71836.5*CLHEP::cm*RAYFF,   65956*CLHEP::cm*RAYFF, 60440.6*CLHEP::cm*RAYFF, 55275.4*CLHEP::cm*RAYFF, 50445.6*CLHEP::cm*RAYFF,
       45937*CLHEP::cm*RAYFF, 41735.2*CLHEP::cm*RAYFF, 37826.6*CLHEP::cm*RAYFF, 34197.6*CLHEP::cm*RAYFF, 30834.9*CLHEP::cm*RAYFF, 
     27725.4*CLHEP::cm*RAYFF, 24856.6*CLHEP::cm*RAYFF, 22215.9*CLHEP::cm*RAYFF, 19791.3*CLHEP::cm*RAYFF, 17570.9*CLHEP::cm*RAYFF,   
       15543*CLHEP::cm*RAYFF, 13696.6*CLHEP::cm*RAYFF, 12020.5*CLHEP::cm*RAYFF, 10504.1*CLHEP::cm*RAYFF, 9137.15*CLHEP::cm*RAYFF,
     7909.45*CLHEP::cm*RAYFF,  6811.3*CLHEP::cm*RAYFF, 5833.25*CLHEP::cm*RAYFF,  4966.2*CLHEP::cm*RAYFF, 4201.36*CLHEP::cm*RAYFF, 
     3530.28*CLHEP::cm*RAYFF, 2944.84*CLHEP::cm*RAYFF, 2437.28*CLHEP::cm*RAYFF, 2000.18*CLHEP::cm*RAYFF,  1626.5*CLHEP::cm*RAYFF, 
     1309.55*CLHEP::cm*RAYFF, 1043.03*CLHEP::cm*RAYFF, 821.016*CLHEP::cm*RAYFF,  637.97*CLHEP::cm*RAYFF, 488.754*CLHEP::cm*RAYFF
   };


   // Get from the tuning parameters
   G4double MIEFF = 0.0; // mie scattering turned off

   //Values extracted from Skdetsim
   G4double MIE_water[NUMENTRIES_water] = {
     7790020*CLHEP::cm*MIEFF, 7403010*CLHEP::cm*MIEFF, 7030610*CLHEP::cm*MIEFF, 6672440*CLHEP::cm*MIEFF, 6328120*CLHEP::cm*MIEFF, 
     5997320*CLHEP::cm*MIEFF, 5679650*CLHEP::cm*MIEFF, 5374770*CLHEP::cm*MIEFF, 5082340*CLHEP::cm*MIEFF, 4802000*CLHEP::cm*MIEFF, 
     4533420*CLHEP::cm*MIEFF, 4276280*CLHEP::cm*MIEFF, 4030220*CLHEP::cm*MIEFF, 3794950*CLHEP::cm*MIEFF, 3570120*CLHEP::cm*MIEFF,
     3355440*CLHEP::cm*MIEFF, 3150590*CLHEP::cm*MIEFF, 2955270*CLHEP::cm*MIEFF, 2769170*CLHEP::cm*MIEFF, 2592000*CLHEP::cm*MIEFF, 
     2423470*CLHEP::cm*MIEFF, 2263300*CLHEP::cm*MIEFF, 2111200*CLHEP::cm*MIEFF, 1966900*CLHEP::cm*MIEFF, 1830120*CLHEP::cm*MIEFF, 
     1700610*CLHEP::cm*MIEFF, 1578100*CLHEP::cm*MIEFF, 1462320*CLHEP::cm*MIEFF, 1353040*CLHEP::cm*MIEFF, 1250000*CLHEP::cm*MIEFF,
     1152960*CLHEP::cm*MIEFF, 1061680*CLHEP::cm*MIEFF,  975936*CLHEP::cm*MIEFF,  895491*CLHEP::cm*MIEFF,  820125*CLHEP::cm*MIEFF, 
      749619*CLHEP::cm*MIEFF,  683760*CLHEP::cm*MIEFF,  622339*CLHEP::cm*MIEFF,  565152*CLHEP::cm*MIEFF,  512000*CLHEP::cm*MIEFF, 
      462688*CLHEP::cm*MIEFF,  417027*CLHEP::cm*MIEFF,  374832*CLHEP::cm*MIEFF,  335923*CLHEP::cm*MIEFF,  300125*CLHEP::cm*MIEFF,
      267267*CLHEP::cm*MIEFF,  237184*CLHEP::cm*MIEFF,  209715*CLHEP::cm*MIEFF,  184704*CLHEP::cm*MIEFF,  162000*CLHEP::cm*MIEFF, 
      141456*CLHEP::cm*MIEFF,  122931*CLHEP::cm*MIEFF,  106288*CLHEP::cm*MIEFF, 91395.2*CLHEP::cm*MIEFF,   78125*CLHEP::cm*MIEFF, 
     66355.2*CLHEP::cm*MIEFF, 55968.2*CLHEP::cm*MIEFF, 46851.2*CLHEP::cm*MIEFF, 38896.2*CLHEP::cm*MIEFF,   32000*CLHEP::cm*MIEFF
   };

   G4double MIE_water_const[3]={0.4,0.,1};// gforward, gbackward, forward backward ratio

   G4MaterialPropertiesTable *myMPT1 = new G4MaterialPropertiesTable();
   myMPT1->AddProperty("RINDEX",   ENERGY_water, RINDEX1,          NUMENTRIES_water);
   myMPT1->AddProperty("ABSLENGTH",ENERGY_water, ABSORPTION_water, NUMENTRIES_water);
   myMPT1->AddProperty("RAYLEIGH", ENERGY_water, RAYLEIGH_water,   NUMENTRIES_water);
   //myMPT1->AddProperty("MIEHG",    ENERGY_water, MIE_water,        NUMENTRIES_water);
   //myMPT1->AddConstProperty("MIEHG_FORWARD",MIE_water_const[0]);
   //myMPT1->AddConstProperty("MIEHG_BACKWARD",MIE_water_const[1]);
   //myMPT1->AddConstProperty("MIEHG_FORWARD_RATIO",MIE_water_const[2]);
   fMaterials[ "water" ]->SetMaterialPropertiesTable(myMPT1);
  
   fMaterials[ "H" ] = nist->FindOrBuildMaterial( "G4_WATER" );
   fMaterials[ "air" ] = nist->FindOrBuildMaterial( "G4_AIR" );
   // tried to pick reasonable numbers... numbers in WCSim were suspect
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
   G4double RAYLEIGH_air[NUMENTRIES_water] =
     { 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m};
   G4double MIE_air[NUMENTRIES_water] =
     { 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m};
   G4double MIE_air_const[3]={0.99,0.99,0.8};// gforward, gbackward, forward backward ratio
   G4MaterialPropertiesTable *myMPT2 = new G4MaterialPropertiesTable();
   myMPT2->AddProperty("RINDEX",    ENERGY_water, RINDEX_air,          NUMENTRIES_water);
   myMPT2->AddProperty("RAYLEIGH",  ENERGY_water, RAYLEIGH_air,        NUMENTRIES_water);
   //myMPT2->AddProperty("MIEHG",     ENERGY_water, MIE_air,             NUMENTRIES_water);
   //myMPT2->AddConstProperty("MIEHG_FORWARD",MIE_air_const[0]);
   //myMPT2->AddConstProperty("MIEHG_BACKWARD",MIE_air_const[1]);
   //myMPT2->AddConstProperty("MIEHG_FORWARD_RATIO",MIE_air_const[2]);
   fMaterials[ "air" ]->SetMaterialPropertiesTable(myMPT2);
   
   a = 118.71 * CLHEP::g/CLHEP::mole;
   G4Element* elSb = new G4Element("Tin","Sn",50, a);
   a = 39.0983 * CLHEP::g/CLHEP::mole;
   G4Element* elK = new G4Element("Potassium","K",19, a);
   a = 132.90545 * CLHEP::g/CLHEP::mole;
   G4Element* elCs = new G4Element("Caesium","Cs",55, a);
   density = ( 1.93 + 7.31 + 0.862 ) / 3.0 * CLHEP::g / CLHEP::cm3;
   G4Material* SbKCs = new G4Material("SbKCs", density, 3 );
   SbKCs->AddElement( elSb, 1 );
   SbKCs->AddElement( elK,  1 );
   SbKCs->AddElement( elCs, 1 );
   fMaterials[ "cathode" ] = SbKCs;
   const G4int NUMENTRIES_cathode =  21;
   G4double ENERGY_cathode[NUMENTRIES_cathode];
   G4double RINDEX_cathode[NUMENTRIES_cathode];
   G4double ABSLENGTH_cathode[NUMENTRIES_cathode];
   G4double EFFICIENCY_cathode[NUMENTRIES_cathode];
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
     EFFICIENCY_cathode[i] = 1.0;
   }   
   G4MaterialPropertiesTable *myMPTcath = new G4MaterialPropertiesTable();
   myMPTcath->AddProperty("RINDEX",   ENERGY_cathode, RINDEX_cathode,     NUMENTRIES_cathode);
   myMPTcath->AddProperty("ABSLENGTH",ENERGY_cathode, ABSLENGTH_cathode,  NUMENTRIES_cathode);
   myMPTcath->AddProperty("EFFICIENCY",ENERGY_cathode, EFFICIENCY_cathode,  NUMENTRIES_cathode);
   fMaterials[ "cathode" ]->SetMaterialPropertiesTable(myMPTcath);
   
   //---Glass
   a = 28.09*CLHEP::g/CLHEP::mole;
   G4Element* elSi = new G4Element("Silicon", "Si", 14., a); 
   a = 16.00*CLHEP::g/CLHEP::mole;
   G4Element* elO = new G4Element("Oxygen","O", 8,a);
   a = 26.98*CLHEP::g/CLHEP::mole;
   G4Element* elAl = new G4Element("Aluminum", "Al", 13, a);  
   density = 2.20*CLHEP::g/CLHEP::cm3;
   G4Material* SiO2 = new G4Material("SiO2",density,2);
   SiO2->AddElement(elSi, 1);
   SiO2->AddElement(elO , 2);
   a = 10.81*CLHEP::g/CLHEP::mole;
   G4Element* elB = new G4Element("Boron", "B", 5, a);  
   density = 2.46*CLHEP::g/CLHEP::cm3;
   G4Material* B2O3 = new G4Material("B2O3",density,2);
   B2O3->AddElement(elB, 2);
   B2O3->AddElement(elO, 3);
   a = 22.99*CLHEP::g/CLHEP::mole;
   G4Element* elNa = new G4Element("Sodium", "Na", 11, a);  
   density = 2.27*CLHEP::g/CLHEP::cm3;
   G4Material* Na2O = new G4Material("Na2O",density,2);
   Na2O->AddElement(elNa, 2);
   Na2O->AddElement(elO, 1);
   density = 4.00*CLHEP::g/CLHEP::cm3;
   G4Material* Al2O3 = new G4Material("Al2O3",density,2);
   Al2O3->AddElement(elAl, 2);
   Al2O3->AddElement(elO, 3);
   density = 2.23*CLHEP::g/CLHEP::cm3;
   G4Material* Glass  = new G4Material("Glass",density,4);
   Glass->AddMaterial(SiO2, 80.6*CLHEP::perCent);
   Glass->AddMaterial(B2O3, 13.0*CLHEP::perCent);
   Glass->AddMaterial(Na2O, 4.0*CLHEP::perCent);
   Glass->AddMaterial(Al2O3, 2.4*CLHEP::perCent);
   fMaterials[ "pmtglass" ] = Glass;
   // Based on analytical formula from http://refractiveindex.info/?shelf=glass&book=HIKARI-BK&page=J-BK7A for Borosilicate glass 
   G4double RINDEX_glass[NUMENTRIES_water] =
     { 1.5110, 1.5112, 1.5114, 1.5116, 1.5118, 
       1.5121, 1.5123, 1.5126, 1.5128, 1.5131, 
       1.5133, 1.5136, 1.5139, 1.5142, 1.5145, 
       1.5148, 1.5152, 1.5155, 1.5159, 1.5163, 
       1.5167, 1.5171, 1.5176, 1.5180, 1.5185, 
       1.5190, 1.5196, 1.5202, 1.5208, 1.5214, 
       1.5221, 1.5228, 1.5236, 1.5244, 1.5253, 
       1.5263, 1.5273, 1.5284, 1.5296, 1.5309, 
       1.5323, 1.5338, 1.5354, 1.5372, 1.5392, 
       1.5414, 1.5438, 1.5465, 1.5495, 1.5528, 
       1.5566, 1.5608, 1.5657, 1.5713, 1.5777, 
       1.5853, 1.5941, 1.6047, 1.6173, 1.6328 }; 
   
   G4double ABSORPTION_glass[NUMENTRIES_water]= 
     { 1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm, 1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,1.0e9*CLHEP::cm,
       1.0e9*CLHEP::cm, 1.0e9*CLHEP::cm };

   G4MaterialPropertiesTable *myMPT5 = new G4MaterialPropertiesTable();
   myMPT5->AddProperty("RINDEX",   ENERGY_water, RINDEX_glass,     NUMENTRIES_water);
   myMPT5->AddProperty("ABSLENGTH",ENERGY_water, ABSORPTION_glass, NUMENTRIES_water);
   fMaterials[ "pmtglass" ]->SetMaterialPropertiesTable(myMPT5);
   
}

/// function to print info about materials available
std::ostream & operator<<( std::ostream& os, const MaterialsMap& mm ){

  std::vector< std::string > matnames;
  mm.get_material_names( matnames );
  for ( const auto & matname : matnames ){
    G4Material* mat = mm[ matname ];
    os << "-----" << matname << "------" << std::endl;
    os << mat;
    os << std::endl;
  }
  return os;
}

