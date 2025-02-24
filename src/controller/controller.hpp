#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "../model/foragerCreature.hpp"
#include "../model/game.hpp"
#include "../model/shooterCreature.hpp"
#include "../model/wildCreature.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

namespace oopLab3 {
class Controller {
  void checkPermission(Entity &entity);

public:
  Game *game;
  Team team;

  Controller(Team team, Game &game);

  void move(Entity &entity, int dx, int dy) { move(entity, Point(dx, dy)); };
  void move(Entity &entity, Point dPoint);

  void attack(Entity &from, Entity &to);
  void heal(Entity &entity);
  void reload(Entity &entity);

  void dropItem(Entity &entity, Item &item, bool checkPerm = true);
  void pickUpItem(Entity &entity, Item &item);

  void endMove();

  Point calculateBullet(int x0, int y0, int x1, int y1);
};
} // namespace oopLab3
#endif // CONTROLLER_HPP