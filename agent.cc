// Copyright 2017 Stefano Pellegrini

#include "agent.h"

#include "utils.h"

Agent::Agent(const Battery* battery, const int* time) :
  battery_(battery), time_(time), total_time_(*time), switched_rooms_(false) {}

Direction Agent::NextDirection(const Perception& p) {
  // spiral first third
  if (*time_ > total_time_/3*2) {
    return SpiralCommand(p);
  }
  else {
    // switch room after some time
    while (!switched_rooms_ && *time_ > total_time_/3) {
      return SwitchRoomCommand(p);
    }

    // go back to charging station in last third
    if (*time_ < total_time_/3) {
      return GoHomeCommand(p);
    }

    // do spiral in (hopefully) the other room
    return SpiralCommand(p);
  }
}

Direction Agent::SpiralCommand(const Perception& p) {
  return Direction::NONE;
}

Direction Agent::SwitchRoomCommand(const Perception& p) {
  return Direction::NONE;
}

Direction Agent::GoHomeCommand(const Perception& p) {
  return Direction::NONE;
}

bool Agent::IsAlive() const {
  return battery_->Level() > 0;
}
