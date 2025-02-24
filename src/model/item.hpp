#ifndef ITEM_HPP
#define ITEM_HPP

#include <cstring>
#include <mutex>

namespace oopLab3 {

class Item {
protected:
  mutable std::mutex mtx;
  char *name;
  int weight;

public:
  Item(const char *name, int weight) : weight(weight) {
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
  };
  virtual ~Item() { delete[] name; }

  char *getName() const {
    std::lock_guard<std::mutex> lock(mtx);
    return name;
  };
  int getWeight() const {
    std::lock_guard<std::mutex> lock(mtx);
    return weight;
  };
};

} // namespace oopLab3

#endif // ITEM_HPP