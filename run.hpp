#ifndef RUN_HPP
#define RUN_HPP

#include <G4UserRunAction.hh>

class RunAction : public G4UserRunAction
{
public:
  void BeginOfRunAction(G4Run const *) override;
  void EndOfRunAction(G4Run const *) override;
};

#endif