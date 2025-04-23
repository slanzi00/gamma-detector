#include "generator.hpp"

#include <G4Event.hh>
#include <G4IonTable.hh>
#include <G4ParticleGun.hh>
#include <Randomize.hh>
#include <G4SystemOfUnits.hh>

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  m_particle_gun = new G4ParticleGun(1);
  m_particle_gun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete m_particle_gun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *an_event)
{
  // for (int i = 0; i < 5000; ++i)
  // {
  //   auto r = 100.0 * cm * std::sqrt(G4UniformRand());
  //   auto phi = 2.0 * CLHEP::pi * G4UniformRand();
  //   auto x = r * std::cos(phi);
  //   auto y = r * std::sin(phi);
  //   auto z = -200.0;

  //   m_particle_gun->SetParticlePosition(G4ThreeVector(x, y, z));

  //   auto ion = G4IonTable::GetIonTable()->GetIon(19, 40, 0.0);
  //   m_particle_gun->SetParticleDefinition(ion);
  //   m_particle_gun->SetParticleCharge(0);
  //   m_particle_gun->SetParticleEnergy(0.0 * keV);
  //   m_particle_gun->GeneratePrimaryVertex(an_event);
  // }

  for (int i = 0; i < 1; ++i)
  {
    auto r = 1* cm * cm * std::sqrt(G4UniformRand());
    auto phi = 2.0 * CLHEP::pi * G4UniformRand();
    auto x = r * std::cos(phi);
    auto y = r * std::sin(phi);
    auto z = -200.0;

    m_particle_gun->SetParticlePosition(G4ThreeVector(x, y, z));

    auto photon = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    m_particle_gun->SetParticleDefinition(photon);
    m_particle_gun->SetParticleEnergy(1460.0 * keV);
    m_particle_gun->GeneratePrimaryVertex(an_event);
  }
}
