#ifndef FORAGER_CREATURE_HPP
#define FORAGER_CREATURE_HPP

#include "entity.hpp"
#include "inventory.hpp"
#include <memory>

namespace oopLab3 {

class ForagerCreature : public Entity {
public:
  Inventory *inventory;

  ForagerCreature(Point pos,
                  int viewRadius,
                  const char *name,
                  double maxHealth,
                  int maxTime,
                  int timeToMove,
                  int maxWeight)
      : Entity(pos, viewRadius, name, maxHealth, maxTime, timeToMove) {
    inventory = new Inventory(maxWeight);
  };
  ForagerCreature(Point pos,
                  int viewRadius,
                  const char *name,
                  double maxHealth,
                  int maxTime,
                  int timeToMove,
                  Inventory &inventory)
      : Entity(pos, viewRadius, name, maxHealth, maxTime, timeToMove),
        inventory(&inventory) {};
  ~ForagerCreature() { delete inventory; }
};

} // namespace oopLab3

#endif // FORAGER_CREATURE_HPP