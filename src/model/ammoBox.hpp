#ifndef AMMO_BOX_HPP
#define AMMO_BOX_HPP

#include "ammoType.hpp"
#include "item.hpp"
#include <mutex>
#include <stdexcept>

namespace oopLab3 {

class AmmoBox : public Item {
  AmmoType ammoType;
  int ammoPerUse;
  int usesLeft;

  mutable std::mutex mtx;

public:
  AmmoBox(const char *name,
          int weight,
          AmmoType ammoType,
          int ammoPerUse,
          int usesLeft)
      : Item(name, weight), ammoType(ammoType), ammoPerUse(ammoPerUse),
        usesLeft(usesLeft) {}

  AmmoType getAmmoType() const {
    std::lock_guard<std::mutex> lock(mtx);
    return ammoType;
  };
  int getAmmoPerUse() const {
    std::lock_guard<std::mutex> lock(mtx);
    return ammoPerUse;
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

#endif // AMMO_BOX_HPP