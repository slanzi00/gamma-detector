#include "run.hpp"
// #include "config.hpp"

#include <G4AnalysisManager.hh>
#include <G4Run.hh>
#include <G4SystemOfUnits.hh>


void RunAction::BeginOfRunAction(const G4Run*)
{
  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->OpenFile("output.root");
  analysis_manager->CreateH1("energy", "Energy", 1000, 0., 10. * eV);
}

void RunAction::EndOfRunAction(const G4Run*)
{
  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->Write();
  analysis_manager->CloseFile();
}
