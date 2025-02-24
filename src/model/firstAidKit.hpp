#ifndef FIRST_AID_KIT_HPP
#define FIRST_AID_KIT_HPP

#include "item.hpp"
#include <stdexcept>

namespace oopLab3 {

class FirstAidKit : public Item {
  int timeToUse;
  double healthPerUse;
  int usesLeft;

public:
  FirstAidKit(const char *name,
              int weight,
              int timeToUse,
              double healthPerUse,
              int usesLeft)
      : Item(name, weight), timeToUse(timeToUse), healthPerUse(healthPerUse),
        usesLeft(usesLeft) {};

  int getTimeToUse() const {
    std::lock_guard<std::mutex> lock(mtx);
    return timeToUse;
  };
  double getHealthPerUse() const {
    std::lock_guard<std::mutex> lock(mtx);
    return healthPerUse;
  };
  int getUsesLeft() const {
    std::lock_guard<std::mutex> lock(mtx);
    return usesLeft;
  };

  void use() {
    std::lock_guard<std::mutex> lock(mtx);
    (usesLeft > 0) ? usesLeft-- : throw std::runtime_error("no uses left");
  };
};

} // namespace oopLab3

#endif // FIRST_AID_KIT_HPP