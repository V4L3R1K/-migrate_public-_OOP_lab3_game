#ifndef HANDS_HPP
#define HANDS_HPP

#include "weapon.hpp"
#include <stdexcept>

namespace oopLab3 {

class Hands {
  double accuracy;
  mutable std::mutex mtx;

public:
  Weapon *weapon;

  Hands(double accuracy) : accuracy(accuracy), weapon(nullptr) {};
  Hands(double accuracy, Weapon &weapon)
      : accuracy(accuracy), weapon(&weapon) {};

  double getAccuracy() const {
    std::lock_guard<std::mutex> lock(mtx);
    return accuracy;
  };

  void pickUp(Weapon &weapon) {
    std::lock_guard<std::mutex> lock(mtx);
    (this->weapon == nullptr) ? this->weapon = &weapon
                              : throw std::runtime_error("hands are busy");
  };
  void drop() {
    std::lock_guard<std::mutex> lock(mtx);
    weapon = nullptr;
  };
};

} // namespace oopLab3

#endif // HANDS_HPP