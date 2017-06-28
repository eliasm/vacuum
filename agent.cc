// Copyright 2017 Stefano Pellegrini

#include "agent.h"

#include "utils.h"

#include <vector>
#include <iostream>

Agent::Agent(const Battery* battery, const int* time) :
  battery_(battery), time_(time), total_time_(*time), switched_rooms_(false),
  last_direction_(Direction::NONE){}

Direction Agent::NextDirection(const Perception& p) {
  // spiral first third
  if (*time_ > total_time_/3*2) {
    last_direction_ =  SpiralCommand(p);
    return last_direction_;
  }
  else {
    // switch room after some time
    while (!switched_rooms_ && *time_ > total_time_/3) {
      last_direction_ = SwitchRoomCommand(p);
      return last_direction_;
    }

    // go back to charging station in last third
    if (*time_ < total_time_/3) {
      last_direction_ = GoHomeCommand(p);
      return last_direction_;
    }

    // do spiral in (hopefully) the other room
    last_direction_ =  SpiralCommand(p);
    return last_direction_;
  }
}

Direction Agent::SpiralCommand(const Perception& p) {
  // get valid directions
  std::vector<Direction> all_directions {
    Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT
  };

  std::vector<Direction> valid_dirs;
  std::vector<Direction> dirt_dirs;
  for (auto &d : all_directions)
  {
    if (p.HasObstacle(d))
    {
      continue;
    }
    valid_dirs.push_back(d);
  }

  if (valid_dirs.empty())
  {
    return Direction::NONE;
  }
  else if (valid_dirs.size() == 1)
  {
    return valid_dirs[0];
  }
  else
  {
    while (1)
    {
      int idx = static_cast<int>(valid_dirs.size() * Random());
      Direction d = valid_dirs[idx];
      if (last_direction_ == Direction::UP && d != Direction::DOWN)
      {
        return d;
      }
      else if (last_direction_ == Direction::DOWN && d != Direction::UP)
      {
        return d;
      }
      else if (last_direction_ == Direction::LEFT && d != Direction::RIGHT)
      {
        return d;
      }
      else if (last_direction_ == Direction::RIGHT && d != Direction::LEFT)
      {
        return d;
      }
      else if (last_direction_ == Direction::NONE)
      {
        return d;
      }
    }


  }




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
