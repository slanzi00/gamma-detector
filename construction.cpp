#include "construction.hpp"
#include "detector.hpp"

#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Sphere.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>

namespace
{
  G4LogicalVolume *create_world()
  {
    auto nist = G4NistManager::Instance();
    auto air = nist->FindOrBuildMaterial("G4_AIR");
    if (!air)
    {
      G4Exception("create_world()", "Material001", FatalException, "Failed to create air material");
    }

    auto world_solid = new G4Box("world_sv", 500. * mm, 500. * mm, 500. * mm);
    auto world_lv = new G4LogicalVolume(world_solid, air, "world_lv");
    world_lv->SetVisAttributes(G4VisAttributes::GetInvisible());
    return world_lv;
  }
}

DetectorConstruction::DetectorConstruction()
    : m_pmt_lv(nullptr), m_crystal_lv(nullptr), m_shell_lv(nullptr), m_base_lv(nullptr),
      m_nai(nullptr), m_aluminum(nullptr), m_glass_pmt(nullptr), m_air(nullptr)
{
  // Initialize materials
  InitializeMaterials();
}

void DetectorConstruction::InitializeMaterials()
{
  auto *nist = G4NistManager::Instance();
  if (!nist)
  {
    G4Exception("DetectorConstruction::InitializeMaterials()",
                "Nist001", FatalException, "Failed to get NIST manager instance");
  }

  // Basic materials
  m_air = nist->FindOrBuildMaterial("G4_AIR");
  m_aluminum = nist->FindOrBuildMaterial("G4_Al");
  m_glass_pmt = nist->FindOrBuildMaterial("G4_Pyrex_Glass"); // Corretto qui

  if (!m_air || !m_aluminum || !m_glass_pmt)
  {
    G4Exception("DetectorConstruction::InitializeMaterials()",
                "Material002", FatalException, "Failed to create basic materials");
  }

  // NaI crystal material
  auto *Na = nist->FindOrBuildElement("Na");
  auto *I = nist->FindOrBuildElement("I");

  if (!Na || !I)
  {
    G4Exception("DetectorConstruction::InitializeMaterials()",
                "Element001", FatalException, "Failed to create elements for NaI");
  }

  m_nai = new G4Material("NaI", 3.67 * g / cm3, 2);
  m_nai->AddElement(Na, 1);
  m_nai->AddElement(I, 1);

  // Setup material properties
  SetupNaIProperties();
  SetupAluminumProperties();
  SetupGlassProperties();
}

void DetectorConstruction::SetupNaIProperties()
{
  if (!m_nai)
  {
    G4Exception("DetectorConstruction::SetupNaIProperties()",
                "Material003", FatalException, "NaI material not initialized");
  }

  auto *mpt = new G4MaterialPropertiesTable();
  const G4int nEntries = 2;
  G4double energy[nEntries] = {2.0 * eV, 3.5 * eV};
  G4double rindex_nai[nEntries] = {1.85, 1.85};
  G4double scint_intensity[nEntries] = {0.0, 1.0};

  mpt->AddProperty("RINDEX", energy, rindex_nai, nEntries);
  mpt->AddProperty("SCINTILLATIONCOMPONENT1", energy, scint_intensity, nEntries);
  mpt->AddConstProperty("SCINTILLATIONYIELD", 55. / keV);
  mpt->AddConstProperty("RESOLUTIONSCALE", 1.0);
  mpt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 230. * ns);
  mpt->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
  m_nai->SetMaterialPropertiesTable(mpt);
}
void DetectorConstruction::SetupAluminumProperties()
{
  if (!m_aluminum)
  {
    G4Exception("DetectorConstruction::SetupAluminumProperties()",
                "Material004", FatalException, "Aluminum material not initialized");
  }

  auto *mpt = new G4MaterialPropertiesTable();
  const G4int nEntries = 2;
  G4double energy[nEntries] = {2.0 * eV, 3.5 * eV};
  G4double reflectivity[nEntries] = {1.0, 1.0};
  G4double efficiency[nEntries] = {1.0, 1.0};

  mpt->AddProperty("REFLECTIVITY", energy, reflectivity, nEntries);
  mpt->AddProperty("EFFICIENCY", energy, efficiency, nEntries);
  m_aluminum->SetMaterialPropertiesTable(mpt);
}

void DetectorConstruction::SetupGlassProperties()
{
  auto *mpt = new G4MaterialPropertiesTable();
  const G4int nEntries = 2;
  G4double energy[nEntries] = {2.0 * eV, 3.5 * eV};
  G4double rindex_glass[nEntries] = {1.5, 1.5};
  G4double pmt_efficiency[nEntries] = {0.2, 0.2};

  mpt->AddProperty("RINDEX", energy, rindex_glass, nEntries);
  mpt->AddProperty("EFFICIENCY", energy, pmt_efficiency, nEntries);
  m_glass_pmt->SetMaterialPropertiesTable(mpt);
}

void DetectorConstruction::ConstructVolumes()
{
  // Geometry Parameters
  G4double crystal_radius = 40.9 * mm;
  G4double crystal_height = 76.0 * mm;
  G4double shell_thickness = 0.5 * mm;
  G4double pmt_height = 5.0 * mm;

  // Create solids
  m_crystal_solid = new G4Tubs("nai_crystal_solid", 0.0, crystal_radius, crystal_height / 2.0, 0.0, 360.0 * deg);
  m_shell_solid = new G4Tubs("al_shell_solid", crystal_radius, crystal_radius + shell_thickness,
                             crystal_height / 2.0, 0.0, 360.0 * deg);
  m_base_solid = new G4Tubs("al_base_solid", 0.0, crystal_radius + shell_thickness,
                            shell_thickness / 2.0, 0.0, 360.0 * deg);
  m_pmt_solid = new G4Tubs("pmt_solid", 0.0, crystal_radius, pmt_height / 2.0, 0.0, 360.0 * deg);

  // Create logical volumes
  m_crystal_lv = new G4LogicalVolume(m_crystal_solid, m_nai, "nai_crystal_lv");
  m_shell_lv = new G4LogicalVolume(m_shell_solid, m_aluminum, "al_shell_lv");
  m_base_lv = new G4LogicalVolume(m_base_solid, m_aluminum, "al_base_lv");
  m_pmt_lv = new G4LogicalVolume(m_pmt_solid, m_glass_pmt, "pmt_lv");

  // Set visualization attributes
  SetVisualAttributes();
}

void DetectorConstruction::SetVisualAttributes()
{
  m_crystal_lv->SetVisAttributes(G4VisAttributes(G4Colour(1.0, 1.0, 0.0))); // yellow
  m_shell_lv->SetVisAttributes(G4VisAttributes(G4Colour(0.7, 0.7, 0.7)));   // gray
  m_base_lv->SetVisAttributes(G4VisAttributes(G4Colour(0.7, 0.7, 0.7)));    // gray
  m_pmt_lv->SetVisAttributes(G4VisAttributes(G4Colour(0.5, 0.5, 0.5)));     // dark gray
}

void DetectorConstruction::PlaceVolumes(G4LogicalVolume *world_lv)
{
  G4double crystal_height = 76.0 * mm;
  G4double shell_thickness = 0.5 * mm;
  G4double pmt_height = 5.0 * mm;

  // Place crystal (centered at origin)
  new G4PVPlacement(nullptr, G4ThreeVector(), m_crystal_lv, "nai_crystal_pv", world_lv, false, 0, true);

  // Place shell (same position as crystal, just larger radius)
  new G4PVPlacement(nullptr, G4ThreeVector(), m_shell_lv, "al_shell_pv", world_lv, false, 0, true);

  // Place base (below crystal)
  new G4PVPlacement(nullptr,
                    G4ThreeVector(0., 0., -crystal_height / 2.0 - shell_thickness / 2.0),
                    m_base_lv, "al_base_pv", world_lv, false, 0, true);

  // Place PMT (above crystal)
  new G4PVPlacement(nullptr,
                    G4ThreeVector(0., 0., crystal_height / 2.0 + pmt_height / 2.0),
                    m_pmt_lv, "pmt_pv", world_lv, false, 0, true);
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
  auto *world_lv = create_world();
  auto *world_pv = new G4PVPlacement(0, {}, world_lv, "world_pv", nullptr, false, 0, true);

  // Build and place detector components
  ConstructVolumes();
  PlaceVolumes(world_lv);

  return world_pv;
}

void DetectorConstruction::ConstructSDandField()
{
  if (!m_pmt_lv)
  {
    G4Exception("DetectorConstruction::ConstructSDandField()",
                "DetectorConstruction001", FatalException,
                "PMT logical volume not created yet");
  }

  auto *detector = new SensitiveDetector(m_pmt_lv->GetName(), 0);
  m_pmt_lv->SetSensitiveDetector(detector);
}