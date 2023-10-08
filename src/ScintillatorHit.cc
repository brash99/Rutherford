#include "ScintillatorHit.hh"
#include "G4UnitsTable.hh"

ScintillatorHit::ScintillatorHit() : G4VHit(), fEdep(0.), fPos(G4ThreeVector())
{}

ScintillatorHit::~ScintillatorHit()
{}

void ScintillatorHit::Print()
{
    G4cout << "Edep: " << std::setw(7) << G4BestUnit(fEdep, "Energy")
           << " Position: " << std::setw(7) << G4BestUnit(fPos, "Length") << G4endl;
}
