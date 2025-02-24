#ifndef INTERFACES_HPP
#define INTERFACES_HPP

#include "point.hpp"

namespace oopLab3 {

class ICanMove {
public:
  virtual void move(int dx, int dy) = 0;
  virtual void move(Point dPoint) = 0;
};

class ICanPunch {
public:
  virtual void punch() = 0;
};

class ICanShoot {
public:
  virtual void shoot() = 0;
};

class ICanReload {
public:
  virtual void reload() = 0;
};

class ICanHeal {
public:
  virtual void heal() = 0;
};

} // namespace oopLab3

#endif // INTERFACES_HPP