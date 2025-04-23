#ifndef USER_ACTION_HPP
#define USER_ACTION_HPP

#include <G4VUserActionInitialization.hh>

class ActionInitialization : public G4VUserActionInitialization
{
public:
  void Build() const override;
};

#endif
