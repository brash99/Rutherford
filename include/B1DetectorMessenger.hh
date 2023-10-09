#ifndef B1DetectorMessenger_h
#define B1DetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

//---------------------------------------------------------------------------

class B1DetectorConstruction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;

//---------------------------------------------------------------------------

class B1DetectorMessenger: public G4UImessenger
{
public:
  B1DetectorMessenger(B1DetectorConstruction*);
  ~B1DetectorMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  B1DetectorConstruction*        fDetector;

  G4UIdirectory*               fDetectorDir;

  G4UIcmdWithAnInteger*        fNumberOfFoilsCmd;
  G4UIcmdWithADouble*          fFoilThicknessCmd;

  G4UIcommand*                 fUpdateCmd;
};

#endif

//---------------------------------------------------------------------------

