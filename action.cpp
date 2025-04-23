#include "action.hpp"
#include "generator.hpp"
#include "run.hpp"

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction{});
  SetUserAction(new RunAction{});
}