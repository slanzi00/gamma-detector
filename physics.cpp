#include "physics.hpp"

#include <G4DecayPhysics.hh>
#include <G4EmStandardPhysics.hh>
#include <G4OpticalPhysics.hh>
#include <G4RadioactiveDecayPhysics.hh>

PhysicsList::PhysicsList()
{
  RegisterPhysics(new G4RadioactiveDecayPhysics{0});
  RegisterPhysics(new G4DecayPhysics{0});
  RegisterPhysics(new G4EmStandardPhysics{0});
  RegisterPhysics(new G4OpticalPhysics{0});
}