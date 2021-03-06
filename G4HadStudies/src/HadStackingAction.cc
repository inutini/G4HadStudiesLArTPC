
#include "HadStackingAction.hh"
#include "HadAnalysis.hh"
#include "G4Track.hh"
#include "G4HadronicProcessStore.hh"
#include "G4Decay.hh"
//#include "G4PiMinusAbsorptionAtRest.hh"
#include "G4NistManager.hh"
#include "HadEventAction.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadStackingAction::HadStackingAction()
{
  killSecondary  = false;
  pname          = ""; 
  elm            = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadStackingAction::~HadStackingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
HadStackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
 G4ClassificationOfNewTrack status = fUrgent;
 EvtManager = G4EventManager::GetEventManager();
 HadEvtAct = (HadEventAction*)(EvtManager -> GetUserEventAction());
 Bool_t procNP = false;
 Bool_t procPi = false;
 Bool_t procK  = false;
 Bool_t decay = false;
 Bool_t piCapt = false;
 //G4cout << " bbbb    " << aTrack->GetStep()->GetPreStepPoint()->GetTotalEnergy()<< G4endl;
//G4cout << " bbbb    " << aTrack->GetStep()->GetTotalEnergyDeposit() <<G4endl;
 if( (aTrack->GetTrackStatus() == fAlive) && (aTrack->GetParentID() == 1) ) {

   //if( aTrack->GetCreatorProcess()->GetProcessName() != "hIoni" ) {
   //     std::cout << "particle " << aTrack->GetDefinition()->GetParticleName() << " process " << aTrack->GetCreatorProcess()->GetProcessName() << std::endl;
   //}

   procNP = (aTrack->GetCreatorProcess()->GetProcessName()=="protonInelastic") || (aTrack->GetCreatorProcess()->GetProcessName()=="neutronInelastic");

   procPi = (aTrack->GetCreatorProcess()->GetProcessName()=="pi+Inelastic") || (aTrack->GetCreatorProcess()->GetProcessName()=="pi-Inelastic");

   procK = (aTrack->GetCreatorProcess()->GetProcessName()=="kaon+Inelastic") || (aTrack->GetCreatorProcess()->GetProcessName()=="kaon-Inelastic");
   
   decay = (aTrack->GetCreatorProcess()->GetProcessName()=="Decay");
   
   piCapt = (aTrack->GetCreatorProcess()->GetProcessName()=="PiMinusAbsorptionAtRest");
   
   if(procNP || procPi || procK) HadEvtAct->AddTrack(aTrack,1);
   
   if(aTrack->GetCreatorProcess()->GetProcessName()=="hadElastic") HadEvtAct->AddTrack(aTrack,2);
   
   if(decay) HadEvtAct->AddTrack(aTrack,3);
   
   if(piCapt) HadEvtAct->AddTrack(aTrack,4);
 }
  //stack or delete secondaries
 if (killSecondary)      status = fKill;

 return status;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
