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
// $Id$
//
/// \file Sphere1PrimaryGeneratorAction.cc
/// \brief Implementation of the Sphere1PrimaryGeneratorAction class

#include "Sphere1PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4Event.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Sphere1PrimaryGeneratorAction* Sphere1PrimaryGeneratorAction::fgInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const Sphere1PrimaryGeneratorAction* Sphere1PrimaryGeneratorAction::Instance()
{
// Static acces function via G4RunManager 

  return fgInstance;
}      

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Sphere1PrimaryGeneratorAction::Sphere1PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction()//,
//  fParticleGun(0)
{
/*
  //use particle gun
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  //?
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e-");
  //G4ParticleDefinition* particle = particleTable->FindParticle(particleName="opticalphoton");
  fParticleGun->SetParticleDefinition(particle);
  //?
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  //?
  fParticleGun->SetParticleEnergy(1.4*MeV);
  //fParticleGun->SetParticleEnergy(3.03798*1e-06*MeV); //optical photon energy @ 392.6nm: 3.1579*1e-06*MeV
*/  

  //or alternatively use HEPEvt interface
  //?
  //G4String hep_input_file = "/home/christoph/geant4/MyG4_applications/Sphere1-build/Generator/EScattering/output/hepevt.EVT";  //nuebar e- EScattering
  //G4String hep_input_file = "/home/christoph/geant4/MyG4_applications/Sphere1-build/Generator/original/DBeta/output/hepevt.EVT"; //2nuBB or 0nuBB
//  G4String hep_input_file = "/u/nobackup/lwinslow/apps/Event_Generators/DBeta/output/hepevt_1_back_to_back/hepevt_1_back_to_back.EVT"; //one back to back Cd116 0nuBB event
//  HEPEvt = new G4HEPEvtInterface(hep_input_file.data());
  const char* filename = "/u/nobackup/lwinslow/elagin/DBeta/output/Se_0vbb_1e6.EVT";//"data/hepevt.EVT";
  G4cout<<"INPUT_FILE = "<<filename<<G4endl;
  HEPEvt = new G4HEPEvtInterface(filename);

  
  fgInstance = this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Sphere1PrimaryGeneratorAction::~Sphere1PrimaryGeneratorAction()
{
//  delete fParticleGun;
  delete HEPEvt;

  fgInstance = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Sphere1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of each event
  //

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.
  

  /*
  G4double envSizeXY = 0;
  G4double envSizeZ = 0;
  G4LogicalVolume* envLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
  G4Box* envBox = NULL;
  if ( envLV ) envBox = dynamic_cast<G4Box*>(envLV->GetSolid());
  if ( envBox ) {
    envSizeXY = envBox->GetXHalfLength()*2.;
    envSizeZ = envBox->GetZHalfLength()*2.;
  }  
  else  {
    G4cerr << "Envelope volume of box shape not found." << G4endl;
    G4cerr << "Perhaps you have changed geometry." << G4endl;
    G4cerr << "The gun will be place in the center." << G4endl;
  }

  G4double size = 0.8; 
  G4double x0 = size * envSizeXY * (G4UniformRand()-0.5);
  G4double y0 = size * envSizeXY * (G4UniformRand()-0.5);
  G4double z0 = -0.5 * envSizeZ;
  */


   
  // Particle Gun
  //?
  G4double x0 = -4500.00;//-6500.000;
  G4double y0 = 0.;
  G4double z0 = 0.;
  //?
  /*
  G4double x0 = 2500.;
  G4double y0 = 2500.;
  G4double z0 = 2500.;
  */

  // optional: set random direction for each primary: spherical coordinate system convention is the same as in stepping action. 
  /* 
  //?
  G4double phi = G4UniformRand()*2.0*pi-pi;
  G4double theta = acos(2.0*G4UniformRand() - 1.0);
  G4double dirx = sin(theta)*cos(phi);
  G4double diry = sin(theta)*sin(phi);
  G4double dirz = cos(theta);  
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(dirx,diry,dirz));  
  G4cout << "-------------------------------" << G4endl;
  G4cout << "G4ThreeVector(dirx,diry,dirz)= " << G4ThreeVector(dirx,diry,dirz) << G4endl;
  G4cout << "phi= " << phi << ", theta= " << theta << G4endl;
  G4cout << "-------------------------------" << G4endl;
  */

  
  //?
/*
  G4ParticleDefinition* ion
	= G4ParticleTable::GetParticleTable()->GetIon(81,208,0); //Tl 
	//= G4ParticleTable::GetParticleTable()->GetIon(90,232,0); //Th series
	//= G4ParticleTable::GetParticleTable()->GetIon(92,238,0); //U series
       //= G4ParticleTable::GetParticleTable()->GetIon(6,10,0);
  fParticleGun->SetParticleDefinition(ion);
  fParticleGun->SetParticleCharge(0.);
*/
/*
  //GunOnly
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
//  SetOptPhotonPolar(); //random polarization, here one can also enter a fixed angle or a special polarization angle distribution
  fParticleGun->GeneratePrimaryVertex(anEvent);
*/  

  
  //HEPEvt generator
  //?
  //HEPEvt->SetParticlePosition(G4ThreeVector(2500.*mm,2500.*mm,2500.*mm));
  HEPEvt->SetParticlePosition(G4ThreeVector(-1500.*mm,0.*mm,0.*mm));
  HEPEvt->GeneratePrimaryVertex(anEvent);
  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Sphere1PrimaryGeneratorAction::SetOptPhotonPolar()
 {
  G4double angle = G4UniformRand() * 360.0*deg;
  //G4cout << "Angle= " << angle << G4endl;
  SetOptPhotonPolar(angle);
 }
 //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void Sphere1PrimaryGeneratorAction::SetOptPhotonPolar(G4double angle)
 {
  if (fParticleGun->GetParticleDefinition()->GetParticleName() != "opticalphoton")
    {
      G4cout << "--> warning from PrimaryGeneratorAction::SetOptPhotonPolar() :"
               "the particleGun is not an opticalphoton" << G4endl;
      return;
    }
                     
  G4ThreeVector normal (1., 0., 0.);
  G4ThreeVector kphoton = fParticleGun->GetParticleMomentumDirection();
  G4ThreeVector product = normal.cross(kphoton); 
  G4double modul2       = product*product;
  
  G4ThreeVector e_perpend (0., 0., 1.);
  if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product; 
  G4ThreeVector e_paralle    = e_perpend.cross(kphoton);
  
  G4ThreeVector polar = std::cos(angle)*e_paralle + std::sin(angle)*e_perpend;
  fParticleGun->SetParticlePolarization(polar);
}

 //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

