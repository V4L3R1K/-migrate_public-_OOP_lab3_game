#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "item.hpp"
#include "matrix.hpp"
#include <algorithm>
#include <stdexcept>

namespace oopLab3 {

class Inventory {
  mutable std::mutex mtx;
  MatrixRow<Item *> *items;
  int maxWeight;

public:
  Inventory(int maxWeight) : maxWeight(maxWeight) {
    items = new MatrixRow<Item *>();
  };

  ~Inventory() { delete items; }

  int getMaxWeight() const {
    std::lock_guard<std::mutex> lock(mtx);
    return maxWeight;
  };

  void dropItem(int index) {
    std::lock_guard<std::mutex> lock(mtx);
    items->erase(items->begin() + index);
  };
  void pickUpItem(Item &item) {
    int cW = getCurrentWeight();
    std::lock_guard<std::mutex> lock(mtx);
    (cW + item.getWeight() <= maxWeight || maxWeight == -1)
        ? items->push_back(&item)
        : throw std::runtime_error("no space left");
  };
  Item *operator[](int index) const {
    std::lock_guard<std::mutex> lock(mtx);
    return items->at(index);
  };
  int getCurrentWeight() {
    std::lock_guard<std::mutex> lock(mtx);
    int sumWeight = 0;
    std::for_each(items->begin(), items->end(), [&sumWeight](const Item *item) {
      sumWeight += item->getWeight();
    });
    return sumWeight;
  };
  int getSize() const {
    std::lock_guard<std::mutex> lock(mtx);
    return (*items).size();
  }
};

} // namespace oopLab3

#endif // INVENTORY_HPP