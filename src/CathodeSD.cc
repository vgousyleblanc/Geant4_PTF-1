#include <iostream>
#include "CathodeSD.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"

// print out step info
void PrintG4Step( G4Step* step ){
  const G4Track              * track    = step->GetTrack();
  const G4ParticleDefinition * particle = track->GetParticleDefinition();
  const G4StepPoint          * pre      = step->GetPreStepPoint();
  const G4ThreeVector        & prepos  = pre->GetPosition();
  const G4VPhysicalVolume    * prevol   = pre->GetPhysicalVolume();
  const G4StepPoint          * post     = step->GetPostStepPoint();
  const G4ThreeVector        & postpos  = post->GetPosition();
  const G4VPhysicalVolume    * postvol  = post->GetPhysicalVolume();

  std::cout << "TrackID = "<<track->GetTrackID()<<" Particle = "<<particle->GetParticleName() << std::endl;
  std::cout << "  Step start = ("
     << prepos.x()/CLHEP::cm<<", "
     << prepos.y()/CLHEP::cm<<", "
     << prepos.z()/CLHEP::cm<<") cm"
     <<" in "<<prevol->GetName()
     << std::endl;
  
  std::cout << "  Step end   = ("
     << postpos.x()/CLHEP::cm<<", "
     << postpos.y()/CLHEP::cm<<", "
     << postpos.z()/CLHEP::cm<<") cm"
     <<" in "<<postvol->GetName()
     << std::endl;
  
  std::cout << " Steplength=" << step->GetStepLength()/CLHEP::cm <<" cm "
     << " firststep="<<step->IsFirstStepInVolume()
     << " laststep="<<step->IsLastStepInVolume()
     << " Edep=" <<step->GetTotalEnergyDeposit()/CLHEP::eV <<" eV "
     << std::endl;
	

}


CathodeSD::CathodeSD( G4String aname ) : G4VSensitiveDetector( aname ), fName( aname ){
    
    G4SDManager * sdman = G4SDManager::GetSDMpointer();
    sdman->AddNewDetector( this );
}

G4bool CathodeSD::ProcessHits( G4Step * step, G4TouchableHistory *  ){

  std::cout<<"CathodeSD::Processhits for "<<fName<<std::endl;
  PrintG4Step(  step );
  return true;
}

/*
void CathodeSD::Initialize( G4HCofThisEvent *  ){

}

void CathodeSD::EndOfEvent( G4HCofThisEvent *  ){

}
*/
