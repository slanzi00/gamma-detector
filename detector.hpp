#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <G4VSensitiveDetector.hh>

#include <string>

class G4Step;
class G4TouchableHistory;

class SensitiveDetector : public G4VSensitiveDetector
{
  int m_detector_id;

 public:
  explicit SensitiveDetector(std::string_view, int);
  bool ProcessHits(G4Step*, G4TouchableHistory*) override;
};

#endif