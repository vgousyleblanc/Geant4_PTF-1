#ifndef _OpticalSurfaceDefinitions_hh_
#define _OpticalSurfaceDefinitions_hh_

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

/// OpticalSurfaceDefinitions
/// Singleton class to set up optical surfaces
/// B. Jamieson, April 2018
class OpticalSurfaceDefinitions {
public:
  class Invalid{ };// throw exception when undefined surface requested
  
  /// Get() single instance of class
  static OpticalSurfaceDefinitions& Get();
  
  /// Operator to get a G4OpticalSurface of given name
  G4OpticalSurface * operator[]( const std::string& key ) const;
  
  /// Setup a skin optical surface using an optical surface of a given name
  void skin_surface( const std::string& surface_name,
		     G4VPhysicalVolume* volume );
  
  /// Setup a border optical surface
  void border_surface( const std::string& surface_name,
		       G4VPhysicalVolume* volume1,
		       G4VPhysicalVolume* volume2 );
  
protected:
  OpticalSurfaceDefinitions();
  
private:
  void BuildSurfaces();
  
  static std::vector< G4LogicalBorderSurface * >     fBorders;
  static std::vector< G4LogicalSkinSurface *>        fSkins;
  static std::map< std::string, G4OpticalSurface * > fSurfaces;
  
  static OpticalSurfaceDefinitions * fOpSurfaceDefs;
};

#endif

