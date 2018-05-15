#ifndef _MaterialsMap_hh_
#define _MaterialsMap_hh_

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "G4Material.hh"

/// MaterialsMap
/// Class to hold map of materials used in experiment
/// B. Jamieson, April 2018
class MaterialsMap {
public:
  class Invalid{ }; // to throw as exception when undefined material is requested

  /// Get() single instance of this class
  static MaterialsMap& Get();

  /// Main operator to get a material (need to know its name)
  /// throws Invalid if given a non-existing material name
  G4Material* operator[]( const std::string& key ) const;
  
  /// Get a vector of material names
  void get_material_names( std::vector< std::string > & matnames ) const;

protected:
  MaterialsMap();

private:
  void   BuildMaterials(); 
  static MaterialsMap * fMaterialsMap;
  static std::map< std::string, G4Material * > fMaterials;
};

/// function to print info about materials available
std::ostream & operator<<( std::ostream& os, const MaterialsMap& mm );







#endif
