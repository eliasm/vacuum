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

    // go back to charging station in last third, or if battery is low
    if (*time_ < total_time_/3 || battery_->Level() < 20) {
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
  Direction direction = Direction::NONE;

  // do not move once we have arrived at the charging station
  // TODO: check whether the battery level stops changing
  const float maximum_noise = 0.5;
  if (p.GetChargingSignal() < maximum_noise) {
    direction = Direction::NONE;
  }
  else {
    // if signal is getting smaller, we are moving in the right direction
    if (last_charging_signal_ > p.GetChargingSignal()) {
      // can we continue in that direction?
      if (p.HasObstacle(last_direction_command_)) {
        direction = last_direction_command_;
      }
      else {
        // just try any other possibility
        // TODO: choose this randomly, instead of bias to the left
        if (!p.HasObstacle(Direction::LEFT)) {
          direction = Direction::LEFT;
        }
        else if (!p.HasObstacle(Direction::RIGHT)) {
          direction = Direction::RIGHT;
        }
        else if (!p.HasObstacle(Direction::UP)) {
          direction = Direction::UP;
        }
        else if (!p.HasObstacle(Direction::DOWN)) {
          direction = Direction::DOWN;
        }
        else {
          direction = Direction::NONE;
        }
      }
    }
    // signal is getting larger
    else {
      switch (last_direction_command_)
      {
        case Direction::UP:
          // can we reverse direction?
          if (!p.HasObstacle(Direction::DOWN)) {
            direction = Direction::DOWN;
          }
          else {
            // try LEFT and RIGHT before UP
            // TODO: choose randomly between LEFT and RIGHT
            if (!p.HasObstacle(Direction::LEFT)) {
              direction = Direction::LEFT;
            }
            else if (!p.HasObstacle(Direction::RIGHT)) {
              direction = Direction::RIGHT;
            }
            else {
              direction = Direction::UP;
            }
          }
          break;
        case Direction::DOWN:
          // can we reverse direction?
          if (!p.HasObstacle(Direction::UP)) {
            direction = Direction::UP;
          }
          else {
            // try LEFT and RIGHT before DOWN
            // TODO: choose randomly between LEFT and RIGHT
            if (!p.HasObstacle(Direction::LEFT)) {
              direction = Direction::LEFT;
            }
            else if (!p.HasObstacle(Direction::RIGHT)) {
              direction = Direction::RIGHT;
            }
            else {
              direction = Direction::DOWN;
            }
          }
          break;
        case Direction::LEFT:
          // can we reverse direction?
          if (!p.HasObstacle(Direction::RIGHT)) {
            direction = Direction::RIGHT;
          }
          else {
            // try UP and DOWN before LEFT
            // TODO: choose randomly between UP and DOWN
            if (!p.HasObstacle(Direction::UP)) {
              direction = Direction::UP;
            }
            else if (!p.HasObstacle(Direction::DOWN)) {
              direction = Direction::DOWN;
            }
            else {
              direction = Direction::LEFT;
            }
          }
          break;
        case Direction::RIGHT:
          // can we reverse direction?
          if (!p.HasObstacle(Direction::LEFT)) {
            direction = Direction::LEFT;
          }
          else {
            // try UP and DOWN before RIGHT
            // TODO: choose randomly between left and right
            if (!p.HasObstacle(Direction::UP)) {
              direction = Direction::UP;
            }
            else if (!p.HasObstacle(Direction::DOWN)) {
              direction = Direction::DOWN;
            }
            else {
              direction = Direction::RIGHT;
            }
          }
          break;
        // fallback
        default:
          if (!p.HasObstacle(Direction::LEFT)) {
            direction = Direction::LEFT;
          }
          else if (!p.HasObstacle(Direction::RIGHT)) {
            direction = Direction::RIGHT;
          }
          else if (!p.HasObstacle(Direction::UP)) {
            direction = Direction::UP;
          }
          else if (!p.HasObstacle(Direction::DOWN)) {
            direction = Direction::DOWN;
          }
          else {
            direction = Direction::NONE;
          }
          break;
      }
    }
  }

  last_direction_command_ = direction;
  last_charging_signal_ = p.GetChargingSignal();

  return direction;
}

bool Agent::IsAlive() const {
  return battery_->Level() > 0;
}
