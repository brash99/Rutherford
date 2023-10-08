// ScintillatorSD.hh
#include "G4VSensitiveDetector.hh"
#include "ScintillatorHit.hh"
#include "G4THitsCollection.hh"

typedef G4THitsCollection<ScintillatorHit> ScintillatorHitsCollection;

extern G4ThreadLocal G4Allocator<ScintillatorHit>* ScintillatorHitAllocator;

class ScintillatorSD : public G4VSensitiveDetector
{
public:
    ScintillatorSD(const G4String& name, const G4String& hitsCollectionName);
    virtual ~ScintillatorSD();

    // Mandatory methods
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
    virtual void Initialize(G4HCofThisEvent* hce);
    virtual void EndOfEvent(G4HCofThisEvent* hce);
private:
    ScintillatorHitsCollection* fHitsCollection;

};

