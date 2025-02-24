#ifndef AI_HPP
#define AI_HPP

#include "controller.hpp"

#include <thread>
#include <vector>

namespace oopLab3 {
void tickTillEnd(Controller &c, bool useMultiThread);
void tick(Controller &c, bool useMultiThread);

void tickForager(Controller &c, size_t index);
void tickShooter(Controller &c, size_t index);
void tickWild(Controller &c, size_t index);

// без многопоточности
void tickSingleThread(Controller &c);

// с многопоточностью
void tickMultiThread(Controller &c);

Soldier *findOps(Controller &c, int viewRadius, Point creaturePos);
Point findWeapon(Controller &c, int viewRadius, Point creaturePos);
Point findItem(Controller &c, int viewRadius, Point creaturePos);
Point findStorage(Controller &c, int viewRadius, Point creaturePos);

} // namespace oopLab3

#endif // AI_HPP