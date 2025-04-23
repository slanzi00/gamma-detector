#ifndef CONSTRUCTION_HPP
#define CONSTRUCTION_HPP

#include <G4VUserDetectorConstruction.hh>

class G4LogicalVolume;
class G4Material;
class G4Tubs;


class DetectorConstruction : public G4VUserDetectorConstruction
{
  G4LogicalVolume *m_pmt_lv;
  G4LogicalVolume *m_crystal_lv;
  G4LogicalVolume *m_shell_lv;
  G4LogicalVolume *m_base_lv;
  G4Tubs *m_crystal_solid;
  G4Tubs *m_shell_solid;
  G4Tubs *m_base_solid;
  G4Tubs *m_pmt_solid;
  G4Material *m_air;
  G4Material *m_nai;
  G4Material *m_aluminum;
  G4Material *m_glass_pmt;
  G4Material *m_glass_pmt_solid;
  G4Material *m_glass_pmt_gas;
  G4Material *m_glass_pmt_liquid;
  G4Material *m_glass_pmt_solid_gas;
  G4Material *m_glass_pmt_solid_liquid;

public:
  DetectorConstruction();
  ~DetectorConstruction() override = default;
  void InitializeMaterials();
  void SetupNaIProperties();
  void SetupAluminumProperties();
  void SetupGlassProperties();
  void ConstructVolumes();
  void SetVisualAttributes();
  void PlaceVolumes(G4LogicalVolume *world_lv);
  G4VPhysicalVolume *Construct() override;
  void ConstructSDandField() override;
};

#endif
