#ifndef SHOOTER_CREATURE_HPP
#define SHOOTER_CREATURE_HPP

#include "entity.hpp"
#include "hands.hpp"
#include "interfaces.hpp"
#include <stdexcept>

namespace oopLab3 {

class ShooterCreature : public Entity, ICanShoot {
public:
  Hands *hands;

  ShooterCreature(Point pos,
                  int viewRadius,
                  const char *name,
                  double maxHealth,
                  int maxTime,
                  int timeToMove,
                  double accuracy)
      : Entity(pos, viewRadius, name, maxHealth, maxTime, timeToMove) {
    hands = new Hands(accuracy);
  };
  ShooterCreature(Point pos,
                  int viewRadius,
                  const char *name,
                  double maxHealth,
                  int maxTime,
                  int timeToMove,
                  Hands &hands)
      : Entity(pos, viewRadius, name, maxHealth, maxTime, timeToMove),
        hands(&hands) {};

  ~ShooterCreature() { delete hands; }

  void shoot() override;
};

} // namespace oopLab3

#endif // SHOOTER_CREATURE_HPP