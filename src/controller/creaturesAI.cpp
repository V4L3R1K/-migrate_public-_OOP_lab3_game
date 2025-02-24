#include "creaturesAI.hpp"

namespace oopLab3 {

Soldier *findOps(Controller &c, int viewRadius, Point creaturePos) {
  int d = viewRadius * viewRadius + 1;
  Soldier *ops = nullptr;
  for (size_t i = 0; i < c.game->entities->size(); i++) {
    Soldier *isOps = dynamic_cast<Soldier *>((*c.game->entities)[i]);
    if (isOps) {
      Point opsPos = isOps->getPos();
      int dx = opsPos.x - creaturePos.x;
      int dy = opsPos.y - creaturePos.y;
      Point endPoint =
          c.calculateBullet(creaturePos.x, creaturePos.y, opsPos.x, opsPos.y);
      if (endPoint.x == opsPos.x && endPoint.y == opsPos.y &&
          dx * dx + dy * dy <= viewRadius * viewRadius &&
          dx * dx + dy * dy < d) {
        ops = isOps;
        d = dx * dx + dy * dy;
      }
    }
  }
  return ops;
}

Point findWeapon(Controller &c, int viewRadius, Point creaturePos) {
  for (size_t y = 0; y < c.game->map->size(); y++)
    for (size_t x = 0; x < ((*c.game->map)[0]).size(); x++)
      for (int i = 0; i < (*c.game->map)[y][x]->inventory->getSize(); i++) {
        int dx = x - creaturePos.x;
        int dy = y - creaturePos.y;
        Weapon *isW =
            dynamic_cast<Weapon *>((*(*c.game->map)[y][x]->inventory)[i]);
        Point endPoint = c.calculateBullet(creaturePos.x, creaturePos.y, x, y);
        if (isW && dx * dx + dy * dy <= viewRadius * viewRadius &&
            endPoint.x == (int)x && endPoint.y == (int)y && isW->getAmmoLeft())
          return endPoint;
      }
  return {-1, -1};
}

Point findItem(Controller &c, int viewRadius, Point creaturePos) {
  for (size_t y = 0; y < c.game->map->size(); y++)
    for (size_t x = 0; x < ((*c.game->map)[0]).size(); x++)
      for (int i = 0; i < (*c.game->map)[y][x]->inventory->getSize(); i++) {
        int dx = x - creaturePos.x;
        int dy = y - creaturePos.y;
        Point endPoint = c.calculateBullet(creaturePos.x, creaturePos.y, x, y);
        if (dx * dx + dy * dy <= viewRadius * viewRadius &&
            endPoint.x == (int)x && endPoint.y == (int)y &&
            (*c.game->map)[y][x]->getCellType() == CellType::Floor)
          return endPoint;
      }
  return {-1, -1};
}

Point findStorage(Controller &c, int viewRadius, Point creaturePos) {
  for (size_t y = 0; y < c.game->map->size(); y++)
    for (size_t x = 0; x < ((*c.game->map)[0]).size(); x++) {
      int dx = x - creaturePos.x;
      int dy = y - creaturePos.y;
      Point endPoint = c.calculateBullet(creaturePos.x, creaturePos.y, x, y);
      if (dx * dx + dy * dy <= viewRadius * viewRadius &&
          endPoint.x == (int)x && endPoint.y == (int)y &&
          (*c.game->map)[y][x]->getCellType() == CellType::Storage)
        return endPoint;
    }
  return {-1, -1};
}

void tickForager(Controller &c, size_t index) {
  ForagerCreature *creature =
      dynamic_cast<ForagerCreature *>((*c.game->entities)[index]);

  Point wPos = findItem(c, creature->getViewRadius(), creature->getPos());
  Point sPos = findStorage(c, creature->getViewRadius(), creature->getPos());
  Weapon *w = nullptr;
  if (wPos.x != -1 && wPos.y != -1)
    for (int i = 0; i < (*c.game->map)[wPos.y][wPos.x]->inventory->getSize();
         i++) {
      Weapon *isW = dynamic_cast<Weapon *>(
          (*(*c.game->map)[wPos.y][wPos.x]->inventory)[i]);
      if (isW)
        w = isW;
    }

  int dx = 0;
  int dy = 0;
  bool drop = false;
  bool pickup = false;

  int dir = std::rand() % 3;
  if (dir == 1)
    dx = -1;
  else if (dir == 2)
    dx = 1;
  dir = std::rand() % 3;
  if (dir == 1)
    dy = -1;
  else if (dir == 2)
    dy = 1;
  if (sPos.x == creature->getPos().x && sPos.y == creature->getPos().y &&
      creature->inventory->getSize() != 0)
    drop = true;
  if (wPos.x == creature->getPos().x && wPos.y == creature->getPos().y &&
      creature->inventory->getSize() == 0)
    pickup = true;
  if (wPos.x != -1 && wPos.x < creature->getPos().x &&
      creature->inventory->getSize() == 0)
    dx = -1;
  if (wPos.x != -1 && wPos.x > creature->getPos().x &&
      creature->inventory->getSize() == 0)
    dx = 1;
  if (wPos.x != -1 && wPos.y < creature->getPos().y &&
      creature->inventory->getSize() == 0)
    dy = -1;
  if (wPos.x != -1 && wPos.y > creature->getPos().y &&
      creature->inventory->getSize() == 0)
    dy = 1;
  if (sPos.x != -1 && sPos.x < creature->getPos().x &&
      creature->inventory->getSize() != 0)
    dx = -1;
  if (sPos.x != -1 && sPos.x > creature->getPos().x &&
      creature->inventory->getSize() != 0)
    dx = 1;
  if (sPos.x != -1 && sPos.y < creature->getPos().y &&
      creature->inventory->getSize() != 0)
    dy = -1;
  if (sPos.x != -1 && sPos.y > creature->getPos().y &&
      creature->inventory->getSize() != 0)
    dy = 1;

  if (creature->getTime() < creature->getTimeToMove()) {
    dx = 0;
    dy = 0;
  }

  if (drop)
    try {
      c.dropItem(*creature, *(*creature->inventory)[0]);
      return;
    } catch (std::runtime_error &e) {
    }
  if (pickup && w)
    try {
      c.pickUpItem(*creature, *w);
      return;
    } catch (std::runtime_error &e) {
    }
  if (dx)
    try {
      c.move(*creature, {dx, 0});
      return;
    } catch (std::runtime_error &e) {
    }
  if (dy)
    try {
      c.move(*creature, {0, dy});
      return;
    } catch (std::runtime_error &e) {
    }
  if (!dx && !dy && !pickup && !drop)
    creature->nullifyTime();
}

void tickShooter(Controller &c, size_t index) {
  ShooterCreature *creature =
      dynamic_cast<ShooterCreature *>((*c.game->entities)[index]);

  Soldier *ops = findOps(c, creature->getViewRadius(), creature->getPos());
  Point wPos = findWeapon(c, creature->getViewRadius(), creature->getPos());
  Weapon *w = nullptr;
  if (wPos.x != -1 && wPos.y != -1)
    for (int i = 0; i < (*c.game->map)[wPos.y][wPos.x]->inventory->getSize();
         i++) {
      Weapon *isW = dynamic_cast<Weapon *>(
          (*(*c.game->map)[wPos.y][wPos.x]->inventory)[i]);
      if (isW && isW->getAmmoLeft())
        w = isW;
    }

  int dx = 0;
  int dy = 0;
  bool shoot = false;
  bool drop = false;
  bool pickup = false;

  if (ops == nullptr && wPos.x == -1 && wPos.y == -1) {
    int dir = std::rand() % 3;
    if (dir == 1)
      dx = -1;
    else if (dir == 2)
      dx = 1;
    dir = std::rand() % 3;
    if (dir == 1)
      dy = -1;
    else if (dir == 2)
      dy = 1;
  } else if (creature->hands->weapon == nullptr && wPos.x != -1 &&
             wPos.y != -1) {
    pickup = true;
    if (wPos.x < creature->getPos().x)
      dx = -1;
    if (wPos.x > creature->getPos().x)
      dx = 1;
    if (wPos.y < creature->getPos().y)
      dy = -1;
    if (wPos.y > creature->getPos().y)
      dy = 1;
  } else if (creature->hands->weapon->getAmmoLeft() == 0)
    drop = true;
  else if (creature->getTime() >= creature->hands->weapon->getTimeToShoot())
    shoot = true;

  if (creature->getTime() < creature->getTimeToMove()) {
    dx = 0;
    dy = 0;
  }

  if (drop)
    try {
      c.dropItem(*creature, *creature->hands->weapon, true);
      return;
    } catch (std::runtime_error &e) {
    }
  if (pickup && w)
    try {
      c.pickUpItem(*creature, *w);
      return;
    } catch (std::runtime_error &e) {
    }
  if (shoot && ops)
    try {
      c.attack(*creature, *ops);
      return;
    } catch (std::runtime_error &e) {
    }
  if (dx)
    try {
      c.move(*creature, {dx, 0});
      return;
    } catch (std::runtime_error &e) {
    }
  if (dy)
    try {
      c.move(*creature, {0, dy});
      return;
    } catch (std::runtime_error &e) {
    }
  if (!shoot && !dx && !dy && !pickup && !drop)
    creature->nullifyTime();
}

void tickWild(Controller &c, size_t index) {
  WildCreature *creature =
      dynamic_cast<WildCreature *>((*c.game->entities)[index]);

  Soldier *ops = findOps(c, creature->getViewRadius(), creature->getPos());

  int dx = 0;
  int dy = 0;
  bool punch = false;

  if (ops == nullptr) {
    int dir = std::rand() % 3;
    if (dir == 1)
      dx = -1;
    else if (dir == 2)
      dx = 1;
    dir = std::rand() % 3;
    if (dir == 1)
      dy = -1;
    else if (dir == 2)
      dy = 1;
  } else {
    if (ops->getPos().x < creature->getPos().x)
      dx = -1;
    if (ops->getPos().x > creature->getPos().x)
      dx = 1;
    if (ops->getPos().y < creature->getPos().y)
      dy = -1;
    if (ops->getPos().y > creature->getPos().y)
      dy = 1;
  }

  if (creature->getTime() >= creature->getTimeToPunch())
    punch = true;
  if (creature->getTime() < creature->getTimeToMove()) {
    dx = 0;
    dy = 0;
  }

  if (punch && ops)
    try {
      c.attack(*creature, *ops);
      return;
    } catch (std::runtime_error &e) {
    }
  if (dx)
    try {
      c.move(*creature, {dx, 0});
      return;
    } catch (std::runtime_error &e) {
    }
  if (dy)
    try {
      c.move(*creature, {0, dy});
      return;
    } catch (std::runtime_error &e) {
    }
  if (!punch && !dx && !dy)
    creature->nullifyTime();
}

void tickTillEnd(Controller &c, bool useMultiThread) {
  int i = 0;
  while (c.game->getCurrentTurn() == Team::Creatures && i < 1000) {
    tick(c, useMultiThread);
    i++;
  }
  if (c.game->getCurrentTurn() == Team::Creatures)
    c.endMove();
}

void tick(Controller &c, bool useMultiThread) {
  if (useMultiThread)
    tickMultiThread(c);
  else
    tickSingleThread(c);
}

void tickSingleThread(Controller &c) {
  bool endMove = true;
  for (size_t i = 0; i < c.game->entities->size(); i++) {
    Entity *entPtr = (*c.game->entities)[i];
    ForagerCreature *isF = dynamic_cast<ForagerCreature *>(entPtr);
    ShooterCreature *isS = dynamic_cast<ShooterCreature *>(entPtr);
    WildCreature *isW = dynamic_cast<WildCreature *>(entPtr);

    if (isF && isF->getTime())
      tickForager(c, i);
    if (isS && isS->getTime())
      tickShooter(c, i);
    if (isW && isW->getTime())
      tickWild(c, i);

    if (entPtr->getTime() && (isF || isS || isW))
      endMove = false;
  }

  if (endMove)
    c.endMove();
}

void tickMultiThread(Controller &c) {
  std::vector<std::jthread> threads;
  bool endMove = true;
  for (size_t i = 0; i < c.game->entities->size(); i++) {
    Entity *entPtr = (*c.game->entities)[i];
    ForagerCreature *isF = dynamic_cast<ForagerCreature *>(entPtr);
    ShooterCreature *isS = dynamic_cast<ShooterCreature *>(entPtr);
    WildCreature *isW = dynamic_cast<WildCreature *>(entPtr);

    if (isF && isF->getTime()) {
      threads.emplace_back([&c, i]() { tickForager(c, i); });
    }
    if (isS && isS->getTime()) {
      threads.emplace_back([&c, i]() { tickShooter(c, i); });
    }
    if (isW && isW->getTime()) {
      threads.emplace_back([&c, i]() { tickWild(c, i); });
    }

    if (entPtr->getTime() && (isF || isS || isW))
      endMove = false;
  }

  if (endMove)
    c.endMove();
}
} // namespace oopLab3