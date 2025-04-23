#include "detector.hpp"

#include <G4AnalysisManager.hh>

SensitiveDetector::SensitiveDetector(std::string_view name, int detector_id)
    : G4VSensitiveDetector{name.data()}, m_detector_id{detector_id}
{
}

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  auto track = step->GetTrack();
  track->SetTrackStatus(fStopAndKill);

  auto energy_deposit = step->GetTotalEnergyDeposit();
  if (energy_deposit > 0.0)
  {
    auto analysis_manager = G4AnalysisManager::Instance();
    analysis_manager->FillH1(m_detector_id, energy_deposit);
  }

  return true;
}