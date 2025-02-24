#include "ammoBox.hpp"
#include "entity.hpp"
#include "firstAidKit.hpp"
#include "shooterCreature.hpp"
#include "soldier.hpp"
#include "wildCreature.hpp"
#include <mutex>

namespace oopLab3 {

void Entity::move(Point dPos) {
  std::lock_guard<std::mutex> lock(mtx);
  health > 0
      ? (time >= timeToMove ? pos += dPos,
         time -= timeToMove
                            : throw std::runtime_error("not enough time"))
      : throw std::runtime_error("dead");
};

void ShooterCreature::shoot() {
  std::lock_guard<std::mutex> lock(mtx);
  health > 0 ? (hands->weapon ? (time >= hands->weapon->getTimeToShoot()
                                 ? hands->weapon->shoot(),
                                 time -= hands->weapon->getTimeToShoot()
                                 : throw std::runtime_error("not enough time"))
                              : throw std::runtime_error("no weapon"))
             : throw std::runtime_error("dead");
}

void Soldier::shoot() {
  std::lock_guard<std::mutex> lock(mtx);
  health > 0 ? (hands->weapon ? (time >= hands->weapon->getTimeToShoot()
                                 ? hands->weapon->shoot(),
                                 time -= hands->weapon->getTimeToShoot()
                                 : throw std::runtime_error("not enough time"))
                              : throw std::runtime_error("no weapon"))
             : throw std::runtime_error("dead");
}

void Soldier::heal() {
  std::lock_guard<std::mutex> lock(mtx);
  if (health <= 0)
    throw std::runtime_error("dead");
  for (int i = 0; i < inventory->getSize(); i++) {
    FirstAidKit *fak = dynamic_cast<FirstAidKit *>((*inventory)[i]);
    if (fak) {
      if (fak->getTimeToUse() <= time && fak->getUsesLeft() > 0) {
        fak->use();
        health = std::min(health + fak->getHealthPerUse(), maxHealth);
        time -= fak->getTimeToUse();
        return;
      }
    }
  }
  throw std::runtime_error("unable to heal");
}

void Soldier::reload() {
  std::lock_guard<std::mutex> lock(mtx);
  if (health <= 0)
    throw std::runtime_error("dead");
  for (int i = 0; i < inventory->getSize(); i++) {
    AmmoBox *box = dynamic_cast<AmmoBox *>((*inventory)[i]);
    if (box && hands->weapon) {
      if (timeToReload <= time && box->getUsesLeft() > 0 &&
          box->getAmmoType() == hands->weapon->getAmmoType()) {
        box->use();
        hands->weapon->reload();
        time -= timeToReload;
        return;
      }
    }
  }
  throw std::runtime_error("unable to reload");
}

void WildCreature::punch() {
  std::lock_guard<std::mutex> lock(mtx);
  if (health <= 0)
    throw std::runtime_error("dead");
  if (timeToPunch <= time)
    time -= timeToPunch;
  else
    throw std::runtime_error("unable to punch");
}
} // namespace oopLab3