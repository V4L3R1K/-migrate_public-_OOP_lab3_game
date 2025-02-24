#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "ammoType.hpp"
#include "item.hpp"
#include <stdexcept>

namespace oopLab3 {

class Weapon : public Item {
  double damage;
  int timeToShoot;
  AmmoType ammoType;
  int maxAmmo;
  int ammoLeft;
  mutable std::mutex mtx;

public:
  Weapon(const char *name,
         int weight,
         double damage,
         int timeToShoot,
         AmmoType ammoType,
         int maxAmmo)
      : Item(name, weight), damage(damage), timeToShoot(timeToShoot),
        ammoType(ammoType), maxAmmo(maxAmmo), ammoLeft(maxAmmo) {};
  Weapon(const char *name,
         int weight,
         double damage,
         int timeToShoot,
         AmmoType ammoType,
         int maxAmmo,
         int ammoLeft)
      : Item(name, weight), damage(damage), timeToShoot(timeToShoot),
        ammoType(ammoType), maxAmmo(maxAmmo), ammoLeft(ammoLeft) {};

  double getDamage() const {
    std::lock_guard<std::mutex> lock(mtx);
    return damage;
  };
  int getTimeToShoot() const {
    std::lock_guard<std::mutex> lock(mtx);
    return timeToShoot;
  };
  AmmoType getAmmoType() const {
    std::lock_guard<std::mutex> lock(mtx);
    return ammoType;
  };
  int getMaxAmmo() const {
    std::lock_guard<std::mutex> lock(mtx);
    return maxAmmo;
  };
  int getAmmoLeft() const {
    std::lock_guard<std::mutex> lock(mtx);
    return ammoLeft;
  };

  void shoot() {
    std::lock_guard<std::mutex> lock(mtx);
    ammoLeft > 0 ? ammoLeft-- : throw std::runtime_error("no ammo left");
  };
  void reload() {
    std::lock_guard<std::mutex> lock(mtx);
    ammoLeft = maxAmmo;
  };
};

} // namespace oopLab3

#endif // WEAPON_HPP