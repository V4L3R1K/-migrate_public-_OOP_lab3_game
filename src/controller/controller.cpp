#include "controller.hpp"

namespace oopLab3 {

void Controller::checkPermission(Entity &entity) {
  Soldier *isOps = dynamic_cast<Soldier *>(&entity);
  if ((team == Team::Ops && !isOps) || (team != Team::Ops && isOps) ||
      (game->getCurrentTurn() != team))
    throw std::runtime_error("wrong team");
}

Controller::Controller(Team team, Game &game) : game(&game), team(team) {};

void Controller::move(Entity &entity, Point dPoint) {
  checkPermission(entity);

  if (entity.getPos().x + dPoint.x < 0 ||
      entity.getPos().x + dPoint.x >= (int)game->map->size() ||
      entity.getPos().x + dPoint.y < 0 ||
      entity.getPos().x + dPoint.y >= (int)(*game->map)[0].size())
    throw std::runtime_error("bad path");

  for (size_t i = 0; i < game->entities->size(); i++)
    if (entity.getPos().x + dPoint.x == (*game->entities)[i]->getPos().x &&
        entity.getPos().y + dPoint.y == (*game->entities)[i]->getPos().y)
      throw std::runtime_error("bad path");

  CellType targetCellType =
      (*game->map)[entity.getPos().y + dPoint.y][entity.getPos().x + dPoint.x]
          ->getCellType();
  if (!(targetCellType == CellType::Floor ||
        targetCellType == CellType::Storage))
    throw std::runtime_error("bad path");

  entity.move(dPoint);
}

void Controller::endMove() {
  if (game->getCurrentTurn() != team)
    throw std::runtime_error("wrong team");

  for (int i = 0; i < (int)game->entities->size(); i++)
    (*game->entities)[i]->restoreTime();
  game->swapTurn();
}

void Controller::attack(Entity &from, Entity &to) {
  checkPermission(from);

  ForagerCreature *isForager = dynamic_cast<ForagerCreature *>(&from);
  if (isForager)
    throw std::runtime_error("unable to attack");

  WildCreature *isWild = dynamic_cast<WildCreature *>(&from);
  if (isWild) {
    if (abs(from.getPos().x - to.getPos().x) <= 1 &&
        abs(from.getPos().y - to.getPos().y) <= 1) {
      isWild->punch();
      to.getDamage(isWild->getMeleeDamage() * isWild->getMeleeAccuracy());
    } else
      throw std::runtime_error("too far");
  }

  ShooterCreature *isShooter = dynamic_cast<ShooterCreature *>(&from);
  if (isShooter) {
    isShooter->shoot();
    Point bulletEnd = calculateBullet(
        from.getPos().x, from.getPos().y, to.getPos().x, to.getPos().y);
    (*game->map)[bulletEnd.y][bulletEnd.x]->getShot();
    if (bulletEnd.x == to.getPos().x && bulletEnd.y == to.getPos().y)
      if (static_cast<double>(std::rand()) / RAND_MAX <
          isShooter->hands->getAccuracy())
        to.getDamage(isShooter->hands->weapon->getDamage());
  }

  Soldier *isOps = dynamic_cast<Soldier *>(&from);
  if (isOps) {
    isOps->shoot();
    Point bulletEnd = calculateBullet(
        from.getPos().x, from.getPos().y, to.getPos().x, to.getPos().y);
    (*game->map)[bulletEnd.y][bulletEnd.x]->getShot();
    if (bulletEnd.x == to.getPos().x && bulletEnd.y == to.getPos().y)
      if (static_cast<double>(std::rand()) / RAND_MAX <
          isOps->hands->getAccuracy())
        to.getDamage(isOps->hands->weapon->getDamage());
  }

  if (to.getHealth() <= 0) {
    Soldier *isOpsTo = dynamic_cast<Soldier *>(&to);
    ForagerCreature *isFTo = dynamic_cast<ForagerCreature *>(&to);
    ShooterCreature *isSTo = dynamic_cast<ShooterCreature *>(&to);

    if (isOpsTo) {
      if (isOpsTo->hands->weapon != nullptr)
        dropItem(to, *isOpsTo->hands->weapon, false);
      for (int i = 0; i < isOpsTo->inventory->getSize(); i++)
        dropItem(to, *(*isOpsTo->inventory)[i], false);
    }
    if (isFTo)
      for (int i = 0; i < isFTo->inventory->getSize(); i++)
        dropItem(to, *(*isFTo->inventory)[i], false);
    if (isSTo && isSTo->hands->weapon != nullptr)
      dropItem(to, *isSTo->hands->weapon, false);

    to.remove();
  }
}

// Bresenham's line algorithm курит в сторонке
Point Controller::calculateBullet(int x0, int y0, int x1, int y1) {

  // уравнение траектории
  double k = (x0 != x1) ? (y0 - y1) / (x0 - x1) : 0;
  double b = y0 - k * x0;

  // определение квадранта полета пули
  int dxSign = (x1 - x0 > 0) ? 1 : -1;
  int dySign = (y1 - y0 > 0) ? 1 : -1;

  // смотрим все пересечения траектории с вертикальными линиями сетки
  for (double x = x0 + 0.5 * dxSign; x * dxSign < x1 * dxSign; x += dxSign) {
    int y = round(k * x + b); // y-координата клеток
    if (y < 0)
      y = 0;
    int xFirst = round(x - 0.5 * dxSign); // x-координата первой клетки
    int xSecond = xFirst + dxSign; // x-координата второй клетки

    // первая клеткка - проверка
    if ((*game->map)[y][xFirst]->getCellType() != CellType::Floor &&
        (*game->map)[y][xFirst]->getCellType() != CellType::Storage)
      return Point(xFirst, y);

    // вторая клеткка - проверка
    if ((*game->map)[y][xSecond]->getCellType() != CellType::Floor &&
        (*game->map)[y][xSecond]->getCellType() != CellType::Storage)
      return Point(xSecond, y);
  }

  // смотрим все пересечения траектории с горизонтальными линиями сетки
  for (double y = y0 + 0.5 * dySign; y * dySign < y1 * dySign; y += dySign) {
    int x = (k != 0) ? round((y - b) / k) : x0; // x-координата клеток
    if (x < 0)
      x = 0;
    int yFirst = round(y - 0.5 * dySign); // y-координата первой клетки
    int ySecond = yFirst + dySign; // y-координата второй клетки

    // первая клеткка - проверка
    if ((*game->map)[yFirst][x]->getCellType() != CellType::Floor &&
        (*game->map)[yFirst][x]->getCellType() != CellType::Storage)
      return Point(x, yFirst);

    // вторая клеткка - проверка
    if ((*game->map)[ySecond][x]->getCellType() != CellType::Floor &&
        (*game->map)[ySecond][x]->getCellType() != CellType::Storage)
      return Point(x, ySecond);
  }

  // пуля долетит
  return Point(x1, y1);
}

void Controller::heal(Entity &entity) {
  checkPermission(entity);
  Soldier *canHeal = dynamic_cast<Soldier *>(&entity);
  if (canHeal)
    canHeal->heal();
  else
    throw std::runtime_error("unable to heal");
}

void Controller::reload(Entity &entity) {
  checkPermission(entity);
  Soldier *canReload = dynamic_cast<Soldier *>(&entity);
  if (canReload)
    canReload->reload();
  else
    throw std::runtime_error("unable to reload");
};

void Controller::dropItem(Entity &entity, Item &item, bool checkPerm) {
  if (checkPerm)
    checkPermission(entity);

  Soldier *isOps = dynamic_cast<Soldier *>(&entity);
  if (isOps) {
    if (isOps->hands->weapon == &item) {
      isOps->hands->drop();
      (*game->map)[entity.getPos().y][entity.getPos().x]->inventory->pickUpItem(
          item);
      return;
    } else
      for (int i = 0; i < isOps->inventory->getSize(); i++)
        if ((*isOps->inventory)[i] == &item) {
          isOps->inventory->dropItem(i);
          (*game->map)[entity.getPos().y][entity.getPos().x]
              ->inventory->pickUpItem(item);
          return;
        }
    throw std::runtime_error("item not found");
  }

  ShooterCreature *isShooter = dynamic_cast<ShooterCreature *>(&entity);
  if (isShooter) {
    if (isShooter->hands->weapon == &item) {
      isShooter->hands->drop();
      (*game->map)[entity.getPos().y][entity.getPos().x]->inventory->pickUpItem(
          item);
      return;
    }
    throw std::runtime_error("item not found");
  };

  ForagerCreature *isForager = dynamic_cast<ForagerCreature *>(&entity);
  if (isForager) {
    for (int i = 0; i < isForager->inventory->getSize(); i++)
      if ((*isForager->inventory)[i] == &item) {
        isForager->inventory->dropItem(i);
        (*game->map)[entity.getPos().y][entity.getPos().x]
            ->inventory->pickUpItem(item);
        return;
      }
    throw std::runtime_error("item not found");
  }
}

void Controller::pickUpItem(Entity &entity, Item &item) {
  checkPermission(entity);

  int index =
      (*game->map)[entity.getPos().y][entity.getPos().x]->inventory->getSize();
  for (int i = 0;
       i <
       (*game->map)[entity.getPos().y][entity.getPos().x]->inventory->getSize();
       i++) {
    if ((*(*game->map)[entity.getPos().y][entity.getPos().x]->inventory)[i] ==
        &item)
      index = i;
  }

  if (index ==
      (*game->map)[entity.getPos().y][entity.getPos().x]->inventory->getSize())
    throw std::runtime_error("item not found");

  Weapon *isWeapon = dynamic_cast<Weapon *>(&item);

  Soldier *isOps = dynamic_cast<Soldier *>(&entity);
  if (isOps) {
    if (isOps->hands->weapon == nullptr && isWeapon) {
      (*game->map)[entity.getPos().y][entity.getPos().x]->inventory->dropItem(
          index);
      isOps->hands->pickUp(*isWeapon);
    } else
      try {
        isOps->inventory->pickUpItem(item);
        (*game->map)[entity.getPos().y][entity.getPos().x]->inventory->dropItem(
            index);
      } catch (std::runtime_error &e) {
      }
  }

  ShooterCreature *isS = dynamic_cast<ShooterCreature *>(&entity);
  if (isS && isS->hands->weapon == nullptr && isWeapon) {
    (*game->map)[entity.getPos().y][entity.getPos().x]->inventory->dropItem(
        index);
    isS->hands->pickUp(*isWeapon);
  }

  ForagerCreature *isF = dynamic_cast<ForagerCreature *>(&entity);
  if (isF)
    try {
      isF->inventory->pickUpItem(item);
      (*game->map)[entity.getPos().y][entity.getPos().x]->inventory->dropItem(
          index);
    } catch (std::runtime_error &e) {
    }
}

} // namespace oopLab3