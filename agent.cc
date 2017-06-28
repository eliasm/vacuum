// Copyright 2017 Stefano Pellegrini

#include "agent.h"

#include "utils.h"

Agent::Agent(const Battery* battery, const int* time) :
  battery_(battery), time_(time), total_time_(*time), switched_rooms_(false),
    lastDir_(Direction::NONE) {}

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
    // Get possible obstacles in every movement direction
    bool obstacleUp = p.HasObstacle(Direction::UP);
    bool obstacleDown = p.HasObstacle(Direction::DOWN);
    bool obstacleRight = p.HasObstacle(Direction::RIGHT);
    bool obstacleLeft = p.HasObstacle(Direction::LEFT);

    if (!obstacleLeft && lastDir_ != Direction::RIGHT) {
        lastDir_ = Direction::LEFT;
    } else if (!obstacleDown && lastDir_ != Direction::UP) {
        lastDir_ = Direction::DOWN;
    } else if (!obstacleRight && lastDir_ != Direction::LEFT) {
        lastDir_ = Direction::RIGHT;
    } else if (!obstacleUp && lastDir_ != Direction::DOWN) {
        lastDir_ = Direction::UP;
    } else {
        // As back and forth cannot be avoided, choose one direction
        // that does not have an obstacle
        if (!obstacleLeft) {
            lastDir_ = Direction::LEFT;
        } else if (!obstacleDown) {
            lastDir_ = Direction::DOWN;
        } else if (!obstacleRight) {
            lastDir_ = Direction::RIGHT;
        } else {
            lastDir_ = Direction::UP;
        }
    }

    return lastDir_;
}

Direction Agent::GoHomeCommand(const Perception& p) {
  return Direction::NONE;
}

bool Agent::IsAlive() const {
  return battery_->Level() > 0;
}
