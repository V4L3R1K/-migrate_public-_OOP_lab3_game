#ifndef WILD_CREATURE_HPP
#define WILD_CREATURE_HPP

#include "entity.hpp"
#include "interfaces.hpp"
#include <stdexcept>

namespace oopLab3 {

class WildCreature : public Entity, ICanPunch {
private:
  int timeToPunch;
  double meleeDamage;
  double meleeAccuracy;

public:
  WildCreature(Point pos,
               int viewRadius,
               const char *name,
               double maxHealth,
               int maxTime,
               int timeToMove,
               int timeToPunch,
               double meleeDamage,
               double meleeAccuracy)
      : Entity(pos, viewRadius, name, maxHealth, maxTime, timeToMove),
        timeToPunch(timeToPunch), meleeDamage(meleeDamage),
        meleeAccuracy(meleeAccuracy) {};

  int getTimeToPunch() const {
    std::lock_guard<std::mutex> lock(mtx);
    return timeToPunch;
  };
  double getMeleeDamage() const {
    std::lock_guard<std::mutex> lock(mtx);
    return meleeDamage;
  };
  double getMeleeAccuracy() const {
    std::lock_guard<std::mutex> lock(mtx);
    return meleeAccuracy;
  };

  void punch() override;
};

} // namespace oopLab3

#endif // WILD_CREATURE_HPP