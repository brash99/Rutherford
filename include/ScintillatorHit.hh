#include "G4VHit.hh"
#include "G4ThreeVector.hh"

class ScintillatorHit : public G4VHit
{
public:
    ScintillatorHit();
    virtual ~ScintillatorHit();
    virtual void Draw() {}
    virtual void Print();

    // ... Additional Methods to store/retrieve information ...
    
    void SetEdep(G4double e) { fEdep = e; }
    G4double GetEdep() { return fEdep; }
    void SetPos(G4ThreeVector p) { fPos = p; }
    G4ThreeVector GetPos() { return fPos; }

private:
    G4double fEdep;  // Energy deposited in this hit
    G4ThreeVector fPos;  // Position of the hit
};

