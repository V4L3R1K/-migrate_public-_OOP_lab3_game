#ifndef SOLDIER_HPP
#define SOLDIER_HPP

#include "entity.hpp"
#include "hands.hpp"
#include "interfaces.hpp"
#include "inventory.hpp"

namespace oopLab3 {

class Soldier : public Entity, ICanHeal, ICanShoot, ICanReload {
private:
  int timeToReload;

public:
  Hands *hands;
  Inventory *inventory;

  Soldier(Point pos,
          int viewRadius,
          const char *name,
          double maxHealth,
          int maxTime,
          int timeToMove,
          int timeToReload,
          double accuracy,
          int maxWeight)
      : Entity(pos, viewRadius, name, maxHealth, maxTime, timeToMove),
        timeToReload(timeToReload) {
    hands = new Hands(accuracy);
    inventory = new Inventory(maxWeight);
  };
  Soldier(Point pos,
          int viewRadius,
          const char *name,
          double maxHealth,
          int maxTime,
          int timeToMove,
          int timeToReload,
          Hands &hands,
          int maxWeight)
      : Entity(pos, viewRadius, name, maxHealth, maxTime, timeToMove),
        timeToReload(timeToReload), hands(&hands) {
    inventory = new Inventory(maxWeight);
  };
  Soldier(Point pos,
          int viewRadius,
          const char *name,
          double maxHealth,
          int maxTime,
          int timeToMove,
          int timeToReload,
          double accuracy,
          Inventory &inventory)
      : Entity(pos, viewRadius, name, maxHealth, maxTime, timeToMove),
        timeToReload(timeToReload), inventory(&inventory) {
    hands = new Hands(accuracy);
  };
  Soldier(Point pos,
          int viewRadius,
          const char *name,
          double maxHealth,
          int maxTime,
          int timeToMove,
          int timeToReload,
          Hands &hands,
          Inventory &inventory)
      : Entity(pos, viewRadius, name, maxHealth, maxTime, timeToMove),
        timeToReload(timeToReload), hands(&hands), inventory(&inventory) {};

  ~Soldier() {
    delete hands;
    delete inventory;
  }

  int getTimeToReload() const {
    std::lock_guard<std::mutex> lock(mtx);
    return timeToReload;
  };

  void reload() override;
  void shoot() override;
  void heal() override;
};

} // namespace oopLab3

#endif // SOLDIER_HPP