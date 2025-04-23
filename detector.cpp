#include "detector.hpp"

#include <G4AnalysisManager.hh>
#include <G4OpticalPhoton.hh>

SensitiveDetector::SensitiveDetector(std::string_view name, int detector_id)
    : G4VSensitiveDetector{name.data()}, m_detector_id{detector_id}
{
}

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  auto track = step->GetTrack();

  if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
  {
    auto time = track->GetGlobalTime();
    auto analysis_manager = G4AnalysisManager::Instance();
    analysis_manager->FillH1(m_detector_id, time);
  }

  track->SetTrackStatus(fStopAndKill);
  return true;
}
