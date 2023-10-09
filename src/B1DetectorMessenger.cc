#include "B1DetectorMessenger.hh"
#include "B1DetectorConstruction.hh"

#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"

//---------------------------------------------------------------------------

B1DetectorMessenger::B1DetectorMessenger(B1DetectorConstruction* Detect)
  :fDetector(Detect)
{

    std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
    std::cout << "In DetectorMessenger Constructor" << std::endl;
    std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

  fDetectorDir        = new G4UIdirectory("/Rutherford/detector/");
  fDetectorDir->SetGuidance("Detector geometry control");
  
  fNumberOfFoilsCmd        = new G4UIcmdWithAnInteger("/Rutherford/detector/NumberOfFoils",this);
  fNumberOfFoilsCmd->SetGuidance("Set the number of gold foils");

  fFoilThicknessCmd        = new G4UIcmdWithADouble("/Rutherford/detector/FoilThickness",this);
  fFoilThicknessCmd->SetGuidance("Set the thickness of the gold foil in cm");

  fUpdateCmd          = new G4UIcommand("/Rutherford/detector/update",this);
  fUpdateCmd->SetGuidance("Update the detector geometry with changed values.");
  fUpdateCmd->SetGuidance("Must be run before beamOn if detector has been changed.");  
}

//---------------------------------------------------------------------------

B1DetectorMessenger::~B1DetectorMessenger()
{
  delete fDetectorDir;

  delete fNumberOfFoilsCmd;
  delete fFoilThicknessCmd;

  delete fUpdateCmd;
}

//---------------------------------------------------------------------------

void B1DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
  std::cout << "In DetectorMessenger SetNewValue:" << command << std::endl;  
  std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;


  if(command == fUpdateCmd)
    fDetector->UpdateGeometry();
  
  if (command == fNumberOfFoilsCmd)
    fDetector->SetNumberOfFoils(fNumberOfFoilsCmd->GetNewIntValue(newValue));
  if (command == fFoilThicknessCmd)
    fDetector->SetFoilThickness(fFoilThicknessCmd->GetNewDoubleValue(newValue));

}

//---------------------------------------------------------------------------
