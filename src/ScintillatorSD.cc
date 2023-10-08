#include "ScintillatorSD.hh"
#include "globalTree.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"

G4ThreadLocal G4Allocator<ScintillatorHit>* ScintillatorHitAllocator = nullptr;

ScintillatorSD::ScintillatorSD(const G4String& name, const G4String& hitsCollectionName)
    : G4VSensitiveDetector(name),
      fHitsCollection(nullptr)
{
    collectionName.insert(hitsCollectionName);
}

ScintillatorSD::~ScintillatorSD() {}

void ScintillatorSD::Initialize(G4HCofThisEvent* hce)
{
    fHitsCollection = new ScintillatorHitsCollection(SensitiveDetectorName, collectionName[0]);
    auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollection);
}

void ScintillatorSD::EndOfEvent(G4HCofThisEvent*)
{
    //if (verboseLevel > 1)
    {
        G4int nofHits = fHitsCollection->entries();
        if (nofHits > 0) {
            G4cout << G4endl << "-------->Hits Collection: in this event they are " << nofHits
                   << " hits in the scintillator: " << G4endl;
            for (G4int i=0; i<nofHits; i++) (*fHitsCollection)[i]->Print();
        }
    }

    if (myTree){
        TBranch* b_edep = myTree->GetBranch("energyDep");
        TBranch* b_xpos = myTree->GetBranch("positionX");
        TBranch* b_ypos = myTree->GetBranch("positionY");
        TBranch* b_zpos = myTree->GetBranch("positionZ");

        G4int nofHits = fHitsCollection->entries();
        if (nofHits > 0) {
            for (G4int i=0; i<nofHits; i++) {
                if (b_edep && b_xpos && b_ypos && b_zpos){
                    auto hit = (*fHitsCollection)[i];
                    G4double edep = hit->GetEdep();
                    G4double xpos = hit->GetPos().x();
                    G4double ypos = hit->GetPos().y();
                    G4double zpos = hit->GetPos().z();

                    b_edep->SetAddress(&edep);
                    b_xpos->SetAddress(&xpos);
                    b_ypos->SetAddress(&ypos);
                    b_zpos->SetAddress(&zpos);

                    myTree->Fill();
                }
            }
        }
    }

}


G4bool ScintillatorSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    G4double edep = aStep->GetTotalEnergyDeposit();
    if (edep == 0.) return false;

    auto preStepPoint = aStep->GetPreStepPoint();
    auto touchable = preStepPoint->GetTouchable();

    auto hit = new ScintillatorHit();
    hit->SetEdep(edep);
    hit->SetPos(aStep->GetPreStepPoint()->GetPosition());
    
    fHitsCollection->insert(hit);



    return true;
}

