#ifndef CELL_HPP
#define CELL_HPP

#include "inventory.hpp"

namespace oopLab3 {

enum class CellType { Floor, Wall, Glass, WallBreakable, Storage };

class Cell {
  CellType cellType;
  mutable std::mutex mtx;

public:
  Inventory *inventory;

  Cell(CellType cellType) : cellType(cellType) {
    inventory = new Inventory(-1);
  };
  Cell(CellType cellType, Inventory &inventory)
      : cellType(cellType), inventory(&inventory) {};

  ~Cell() { delete inventory; }

  CellType getCellType() const {
    std::lock_guard<std::mutex> lock(mtx);
    return cellType;
  };

  void getShot() {
    std::lock_guard<std::mutex> lock(mtx);
    if (cellType == CellType::WallBreakable || cellType == CellType::Glass)
      cellType = CellType::Floor;
  };
};

} // namespace oopLab3

#endif // CELL_HPP