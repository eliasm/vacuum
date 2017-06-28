#ifndef AGENT_H_
#define AGENT_H_

#include "battery.h"
#include "geometry.h"
#include "map.h"
#include "perception.h"

// Models a vacuum cleaner.
class Agent {
 public:
  // Constructs an agent with a |battery| and a |time| that are read
  // only. Both arguments should outlive the agent.
  Agent(const Battery* battery, const int* time);

  // Get the next direction of the agent based on the current perception |p|.
  Direction NextDirection(const Perception& p);

  // Whether the agent has still some battery left.
  bool IsAlive() const;

 private:
  // Not owned by the agent.
  const Battery* battery_;
  // Not owned by the agent.
  const int* time_;

  // Maximum time that the agent has available
  const int total_time_;
  // Did we switch the room?
  bool switched_rooms_;

  // variables for spiral move
  Direction last_direction_;


  Direction SpiralCommand(const Perception& p);
  Direction SwitchRoomCommand(const Perception& p);
  Direction GoHomeCommand(const Perception& p);


};

#endif  // AGENT_H_
