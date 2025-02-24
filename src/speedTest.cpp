#include "view/view.hpp"
#include <chrono>
#include <iostream>

#define MAP_SIZE 25
#define SPAWN_OFFSET 10

#define N_ENTITIES_MIN 1
#define N_ENTITIES_MAX 200
#define N_TESTS 100

Matrix<Cell *> generateMap(int w, int h) {
  Matrix<Cell *> map = {};
  for (int y = 0; y < w; y++) {
    map.push_back({});
    for (int x = 0; x < h; x++) {
      if (x == 0 || y == 0 || x == h - 1 || y == w - 1)
        map[map.size() - 1].push_back(new Cell(CellType::Wall));
      else
        map[map.size() - 1].push_back(new Cell(CellType::Floor));
    }
  }
  return map;
}

double doOneTest(int nEntities, bool useMultiThread, Matrix<Cell *> *map) {
  MatrixRow<Entity *> entities;
  for (int i = 0; i < nEntities; i++)
    entities.push_back(new ForagerCreature(
        {1 + SPAWN_OFFSET + std::rand() % (MAP_SIZE - 2 - SPAWN_OFFSET * 2),
         1 + SPAWN_OFFSET + std::rand() % (MAP_SIZE - 2 - SPAWN_OFFSET * 2)},
        0,
        "",
        1,
        100,
        10,
        0));
  Game g = Game(*map, entities, Team::Creatures);
  Controller c = Controller(Team::Creatures, g);

  auto start = std::chrono::high_resolution_clock::now();
  tick(c, useMultiThread);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;

  for (auto &ent : entities)
    delete ent;

  return duration.count();
}

int main() {
  Matrix<Cell *> map = generateMap(MAP_SIZE, MAP_SIZE);

  std::cout << "n_entities time_single time_multi" << std::endl;
  for (int i = 0; i < N_TESTS; i++) {
    int n =
        N_ENTITIES_MIN + i * (N_ENTITIES_MAX - N_ENTITIES_MIN) / (N_TESTS - 1);
    double s = doOneTest(n, false, &map);
    double m = doOneTest(n, true, &map);
    std::cout << n << " " << s * 1000 << " " << m * 1000 << std::endl;
  }

  for (auto &row : map)
    for (auto &cell : row)
      delete cell;
  return 0;
}