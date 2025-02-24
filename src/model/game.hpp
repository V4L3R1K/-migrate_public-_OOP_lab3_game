#ifndef GAME_HPP
#define GAME_HPP

#include "cell.hpp"
#include "entity.hpp"
#include "matrix.hpp"
#include "soldier.hpp"
#include "team.hpp"

namespace oopLab3 {

class Game {
  Team currentTurn;
  mutable std::mutex mtx;

public:
  Matrix<Cell *> *map;
  MatrixRow<Entity *> *entities;

  Game(Matrix<Cell *> &map, MatrixRow<Entity *> &entities, Team currentTurn)
      : currentTurn(currentTurn), map(&map), entities(&entities) {
    for (int i = 0; i < (int)(entities.size()); i++)
      (entities[i])->restoreTime();
  };

  Team getCurrentTurn() {
    std::lock_guard<std::mutex> lock(mtx);
    return currentTurn;
  };

  Team whoWon() {
    std::lock_guard<std::mutex> lock(mtx);
    bool OpsDead = true, CreaturesDead = true;
    for (int i = 0; i < (int)entities->size(); i++)
      if ((*entities)[i]->getHealth() > 0) {
        Soldier *isOps = dynamic_cast<Soldier *>((*entities)[i]);
        isOps ? OpsDead = false : CreaturesDead = false;
      }
    if (CreaturesDead)
      return Team::Ops;
    if (OpsDead)
      return Team::Creatures;
    throw std::runtime_error("game not finished");
  };

  void swapTurn() {
    std::lock_guard<std::mutex> lock(mtx);
    currentTurn =
        (currentTurn == Team::Creatures) ? Team::Ops : Team::Creatures;
  };
};

} // namespace oopLab3

#endif // GAME_HPP