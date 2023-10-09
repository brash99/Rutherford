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
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"
#include "ScintillatorSD.hh"
#include "G4SDManager.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1DetectorConstruction::ConstructSDandField()
{
    ScintillatorSD* scintSD = new ScintillatorSD("/mydetectors/scintillator", "ScintHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(scintSD);
    SetSensitiveDetector("ScintillatorLog", scintSD);  // Assumes the logical volume of your detector is named "ScintillatorLog"
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{

  // Define a generic scintillator material
  G4double density = 1.032*g/cm3;
  G4Material* ScintillatorMaterial = new G4Material("ScintillatorMat", density, 2); 
  G4Element* elH = new G4Element("Hydrogen", "H", 1., 1.01*g/mole);
  G4Element* elC = new G4Element("Carbon",   "C", 6., 12.01*g/mole);
  ScintillatorMaterial->AddElement(elC, 9);
  ScintillatorMaterial->AddElement(elH, 10);


  // Define the geometry of the scintillator
  G4double scintX = 1*cm;
  G4double scintY = 1*cm;
  G4double scintZ = 1*cm;
  G4Box* ScintillatorBox = new G4Box("ScintillatorBox", scintX/2, scintY/2, scintZ/2);

  G4LogicalVolume* ScintillatorLog = new G4LogicalVolume(ScintillatorBox, ScintillatorMaterial, "ScintillatorLog");
  

  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeXY = 5*cm, env_sizeZ = 10*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.05*env_sizeXY;
  G4double world_sizeZ  = 1.05*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
                                             //
  // Gold Foil
  //

  G4Material* gold = nist->FindOrBuildMaterial("G4_Au");
  G4double goldThickness = 0.000065;
  //G4double goldThickness = 0.00000065;
  G4Box* solidGold = new G4Box("Gold", 0.5*cm, 0.5*cm, goldThickness*cm/2.0);
    

  G4int nfoils = 40;

  G4LogicalVolume* logicGold = new G4LogicalVolume(solidGold, gold, "Gold");
  for (int i = 0; i<nfoils; i++){

        new G4PVPlacement(0, G4ThreeVector(0, 0, goldThickness*i), logicGold, "Gold", logicEnv, false, i, checkOverlaps);
  
  }
 
  G4double distance = 0.7*cm;
  G4double rotationAngle = -90*deg;
  G4double zposition = distance*cos(rotationAngle);
  G4double xposition = -distance*sin(rotationAngle);

  G4RotationMatrix* rotationMatrix = new G4RotationMatrix();
  rotationMatrix->rotateY(rotationAngle);


  G4ThreeVector pos_scint = G4ThreeVector(xposition+0.5*cm, 0*cm, zposition+0.7*cm);
  
  new G4PVPlacement(rotationMatrix,           // rotation around y axis
                  pos_scint,                  // at position calculated above
                  ScintillatorLog,            // logical volume
                  "ScintillatorPhys",         // name
                  logicEnv,                   // mother  volume
                  false,                      // no boolean operations
                  0,                          // copy number
                  checkOverlaps);             // overlaps checking



  // Set Scintillator logical volume as scoring volume
  //
  fScoringVolume = ScintillatorLog;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
