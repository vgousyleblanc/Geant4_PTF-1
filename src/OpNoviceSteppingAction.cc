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
// $Id: OpNoviceSteppingAction.cc 71007 2013-06-09 16:14:59Z maire $
//
/// \file OpNoviceSteppingAction.cc
/// \brief Implementation of the OpNoviceSteppingAction class

#include "OpNoviceSteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "CathodeSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


OpNoviceSteppingAction::OpNoviceSteppingAction()
: G4UserSteppingAction()
{ 
  fScintillationCounter = 0;
  fCerenkovCounter      = 0;
  fEventNumber = -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceSteppingAction::~OpNoviceSteppingAction()
{ ; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// print out step info
void PrintG4Step( const G4Step* step ){
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



void OpNoviceSteppingAction::UserSteppingAction(const G4Step* step)
{
  G4int eventNumber = G4RunManager::GetRunManager()->
                                              GetCurrentEvent()->GetEventID();

  if (eventNumber != fEventNumber) {
    ///G4cout << " Number of Scintillation Photons in previous event: "
    //        << fScintillationCounter << G4endl;
    // G4cout << " Number of Cerenkov Photons in previous event: "
    //        << fCerenkovCounter << G4endl;
     fEventNumber = eventNumber;
     fScintillationCounter = 0;
     fCerenkovCounter = 0;
  }

  G4Track* track = step->GetTrack();

  G4String ParticleName = track->GetDynamicParticle()->
                                 GetParticleDefinition()->GetParticleName();

  if (ParticleName == "opticalphoton") {

    //PrintG4Step( step );

  }

  const std::vector<const G4Track*>* secondaries =
                                            step->GetSecondaryInCurrentStep();

  if (secondaries->size()>0) {
     for(unsigned int i=0; i<secondaries->size(); ++i) {
        if (secondaries->at(i)->GetParentID()>0) {
           if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition()
               == G4OpticalPhoton::OpticalPhotonDefinition()){
              if (secondaries->at(i)->GetCreatorProcess()->GetProcessName()
               == "Scintillation")fScintillationCounter++;
              if (secondaries->at(i)->GetCreatorProcess()->GetProcessName()
               == "Cerenkov")fCerenkovCounter++;
           }
        }
     }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
