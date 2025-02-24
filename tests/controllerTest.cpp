#define CATCH_CONFIG_MAIN

#include "../src/controller/controller.hpp"
#include "../src/model/ammoBox.hpp"
#include "../src/model/firstAidKit.hpp"
#include "../src/model/foragerCreature.hpp"
#include "../src/model/weapon.hpp"

#include <catch2/catch_all.hpp>
#include <string>

using namespace oopLab3;

Matrix<Cell *> createCustomMap() {
  /*# # # # # # #x
    G . . # . . #
    G . * . * . #
    G . . S . . #
    G * . . . * #
    G . # . # . #
    # # # # # # #
    y             */
  return {{new Cell(CellType::Wall),
           new Cell(CellType::Wall),
           new Cell(CellType::Wall),
           new Cell(CellType::Wall),
           new Cell(CellType::Wall),
           new Cell(CellType::Wall),
           new Cell(CellType::Wall)},
          {new Cell(CellType::Glass),
           new Cell(CellType::Floor),
           new Cell(CellType::Floor),
           new Cell(CellType::Wall),
           new Cell(CellType::Floor),
           new Cell(CellType::Floor),
           new Cell(CellType::Wall)},
          {new Cell(CellType::Glass),
           new Cell(CellType::Floor),
           new Cell(CellType::WallBreakable),
           new Cell(CellType::Floor),
           new Cell(CellType::WallBreakable),
           new Cell(CellType::Floor),
           new Cell(CellType::Wall)},
          {new Cell(CellType::Glass),
           new Cell(CellType::Floor),
           new Cell(CellType::Floor),
           new Cell(CellType::Storage),
           new Cell(CellType::Floor),
           new Cell(CellType::Floor),
           new Cell(CellType::Wall)},
          {new Cell(CellType::Glass),
           new Cell(CellType::WallBreakable),
           new Cell(CellType::Floor),
           new Cell(CellType::Floor),
           new Cell(CellType::Floor),
           new Cell(CellType::WallBreakable),
           new Cell(CellType::Wall)},
          {new Cell(CellType::Glass),
           new Cell(CellType::Floor),
           new Cell(CellType::Wall),
           new Cell(CellType::Floor),
           new Cell(CellType::Wall),
           new Cell(CellType::Floor),
           new Cell(CellType::Wall)},
          {new Cell(CellType::Wall),
           new Cell(CellType::Wall),
           new Cell(CellType::Wall),
           new Cell(CellType::Wall),
           new Cell(CellType::Wall),
           new Cell(CellType::Wall),
           new Cell(CellType::Wall)}};
}

TEST_CASE("move") {
  Matrix<Cell *> map = createCustomMap();
  MatrixRow<Entity *> entities;
  Soldier s = Soldier({2, 3}, 10, "s", 100, 100, 10, 10, 0.9, 1000);
  ForagerCreature f = ForagerCreature({4, 1}, 10, "f", 100, 100, 10, 1000);
  entities.push_back(&s);
  entities.push_back(&f);
  Game g = Game(map, entities, Team::Ops);
  Controller cOps = Controller(Team::Ops, g);
  Controller cCreatures = Controller(Team::Creatures, g);

  REQUIRE_THROWS(cCreatures.move(s, 1, 0));
  REQUIRE_THROWS(cCreatures.move(f, 1, 0));
  REQUIRE_THROWS(cOps.move(f, 1, 0));
  REQUIRE_THROWS(cCreatures.endMove());
  REQUIRE_NOTHROW(cOps.endMove());
  REQUIRE(g.getCurrentTurn() == Team::Creatures);
  REQUIRE_THROWS(cOps.move(s, 1, 0));
  REQUIRE_THROWS(cOps.move(f, 1, 0));
  REQUIRE_THROWS(cCreatures.move(s, 1, 0));

  REQUIRE_THROWS(cCreatures.move(f, 100, 0));
  REQUIRE_THROWS(cCreatures.move(f, -100, 0));
  REQUIRE_THROWS(cCreatures.move(f, 0, 100));
  REQUIRE_THROWS(cCreatures.move(f, 0, -100));

  REQUIRE_THROWS(cCreatures.move(f, 0, -1));

  REQUIRE_NOTHROW(cCreatures.endMove());
  REQUIRE_NOTHROW(cOps.move(s, 1, 0));
  REQUIRE_NOTHROW(cOps.move(s, 1, 0));
  REQUIRE(s.getPos().x == 4);

  for (auto &row : map) {
    for (auto &cell : row) {
      delete cell;
    }
  }
}

TEST_CASE("shoot") {
  Matrix<Cell *> map = createCustomMap();
  MatrixRow<Entity *> entities;
  Soldier s = Soldier({2, 1}, 10, "s", 100, 100, 10, 10, 1, 1000);
  ShooterCreature f = ShooterCreature({2, 4}, 10, "s", 100, 100, 10, 0);
  ShooterCreature f2 = ShooterCreature({1, 2}, 10, "s", 100, 100, 10, 1);
  entities.push_back(&s);
  entities.push_back(&f);
  entities.push_back(&f2);
  Weapon w1 = Weapon("w1", 10, 10, 10, AmmoType::Rifle, 10);
  Weapon w2 = Weapon("w2", 10, 10, 10, AmmoType::Rifle, 10);
  Weapon w3 = Weapon("w3", 10, 10, 10, AmmoType::Rifle, 10);
  s.hands->pickUp(w1);
  f.hands->pickUp(w2);
  f2.hands->pickUp(w3);
  Game g = Game(map, entities, Team::Ops);
  Controller cOps = Controller(Team::Ops, g);
  Controller cCreatures = Controller(Team::Creatures, g);

  REQUIRE_THROWS(cCreatures.attack(s, f));
  REQUIRE_THROWS(cCreatures.attack(f, s));
  REQUIRE_THROWS(cOps.attack(f, s));
  REQUIRE_NOTHROW(cOps.endMove());
  REQUIRE_THROWS(cOps.attack(s, f));
  REQUIRE_THROWS(cOps.attack(f, s));
  REQUIRE_THROWS(cCreatures.attack(s, f));
  REQUIRE_NOTHROW(cCreatures.endMove());

  REQUIRE(map[2][2]->getCellType() == CellType::WallBreakable);
  REQUIRE_NOTHROW(cOps.attack(s, f));
  REQUIRE(map[2][2]->getCellType() == CellType::Floor);
  REQUIRE(f.getHealth() == 100);
  REQUIRE_NOTHROW(cOps.attack(s, f));
  REQUIRE(f.getHealth() == 90);
  REQUIRE_NOTHROW(cOps.endMove());
  REQUIRE_NOTHROW(cCreatures.attack(f, s));
  REQUIRE(s.getHealth() == 100);

  REQUIRE_NOTHROW(cCreatures.attack(f2, s));
  REQUIRE(s.getHealth() == 90);

  for (auto &row : map) {
    for (auto &cell : row) {
      delete cell;
    }
  }
}

TEST_CASE("foragerAttack") {
  Matrix<Cell *> map = createCustomMap();
  MatrixRow<Entity *> entities;
  Soldier s = Soldier({2, 1}, 10, "s", 100, 100, 10, 10, 1, 1000);
  ForagerCreature f = ForagerCreature({1, 1}, 10, "f", 100, 100, 10, 1000);
  entities.push_back(&s);
  entities.push_back(&f);
  Weapon w1 = Weapon("w1", 10, 10, 10, AmmoType::Rifle, 10);
  s.hands->pickUp(w1);
  Game g = Game(map, entities, Team::Creatures);
  Controller cCreatures = Controller(Team::Creatures, g);

  REQUIRE_THROWS(cCreatures.attack(f, s));

  for (auto &row : map) {
    for (auto &cell : row) {
      delete cell;
    }
  }
}

TEST_CASE("wildAttack") {
  Matrix<Cell *> map = createCustomMap();
  MatrixRow<Entity *> entities;
  Soldier s = Soldier({2, 1}, 10, "s", 100, 100, 10, 10, 1, 1000);
  WildCreature f = WildCreature({1, 1}, 10, "f", 100, 100, 10, 10, 10, 0);
  WildCreature f2 = WildCreature({1, 3}, 10, "f", 100, 100, 10, 10, 10, 1);
  entities.push_back(&s);
  entities.push_back(&f);
  entities.push_back(&f2);
  Weapon w1 = Weapon("w1", 10, 10, 10, AmmoType::Rifle, 10);
  s.hands->pickUp(w1);
  Game g = Game(map, entities, Team::Creatures);
  Controller cCreatures = Controller(Team::Creatures, g);

  REQUIRE_THROWS(cCreatures.attack(f2, s));
  REQUIRE_NOTHROW(cCreatures.attack(f, s));
  REQUIRE(s.getHealth() == 100);
  REQUIRE_NOTHROW(cCreatures.move(f2, 0, -1));
  REQUIRE_NOTHROW(cCreatures.attack(f2, s));
  REQUIRE(s.getHealth() == 90);

  for (auto &row : map) {
    for (auto &cell : row) {
      delete cell;
    }
  }
}

TEST_CASE("heal") {
  Matrix<Cell *> map = createCustomMap();
  MatrixRow<Entity *> entities;
  Soldier s = Soldier({2, 1}, 10, "s", 100, 100, 10, 10, 1, 1000);
  ForagerCreature f = ForagerCreature({1, 1}, 10, "f", 100, 100, 10, 1000);
  entities.push_back(&s);
  entities.push_back(&f);
  Game g = Game(map, entities, Team::Ops);
  Controller cOps = Controller(Team::Ops, g);
  Controller cCreatures = Controller(Team::Creatures, g);
  s.getDamage(50);
  f.getDamage(50);
  FirstAidKit fak = FirstAidKit("fak", 10, 10, 10, 4);
  FirstAidKit fak2 = FirstAidKit("fak2", 10, 10, 10, 4);
  s.inventory->pickUpItem(fak);
  f.inventory->pickUpItem(fak2);

  REQUIRE(s.getHealth() == 50);
  REQUIRE_THROWS(cOps.heal(f));
  REQUIRE_THROWS(cCreatures.heal(s));
  REQUIRE_THROWS(cCreatures.heal(f));
  REQUIRE_NOTHROW(cOps.heal(s));
  REQUIRE(s.getHealth() == 60);
  REQUIRE_NOTHROW(cOps.endMove());
  REQUIRE_THROWS(cOps.heal(f));
  REQUIRE_THROWS(cCreatures.heal(s));
  REQUIRE_THROWS(cCreatures.heal(f));
  REQUIRE_THROWS(cOps.heal(s));

  for (auto &row : map) {
    for (auto &cell : row) {
      delete cell;
    }
  }
}

TEST_CASE("reload") {
  Matrix<Cell *> map = createCustomMap();
  MatrixRow<Entity *> entities;
  Soldier s = Soldier({2, 1}, 10, "s", 100, 100, 10, 10, 1, 1000);
  ShooterCreature f = ShooterCreature({1, 1}, 10, "f", 100, 100, 10, 1);
  entities.push_back(&s);
  entities.push_back(&f);
  Game g = Game(map, entities, Team::Ops);
  Controller cOps = Controller(Team::Ops, g);
  Controller cCreatures = Controller(Team::Creatures, g);
  Weapon w1 = Weapon("w1", 10, 10, 10, AmmoType::Rifle, 10, 8);
  Weapon w2 = Weapon("w2", 10, 10, 10, AmmoType::Rifle, 10, 8);
  AmmoBox ab1 = AmmoBox("ab1", 10, AmmoType::Rifle, 10, 10);

  s.hands->pickUp(w1);
  f.hands->pickUp(w2);
  s.inventory->pickUpItem(ab1);

  REQUIRE(w1.getAmmoLeft() == 8);
  REQUIRE_THROWS(cOps.reload(f));
  REQUIRE_THROWS(cCreatures.reload(s));
  REQUIRE_THROWS(cCreatures.reload(f));
  REQUIRE_NOTHROW(cOps.reload(s));
  REQUIRE(w1.getAmmoLeft() == 10);
  REQUIRE(ab1.getUsesLeft() == 9);
  REQUIRE_NOTHROW(cOps.endMove());
  REQUIRE_THROWS(cOps.reload(f));
  REQUIRE_THROWS(cCreatures.reload(s));
  REQUIRE_THROWS(cCreatures.reload(f));
  REQUIRE_THROWS(cOps.reload(s));

  for (auto &row : map) {
    for (auto &cell : row) {
      delete cell;
    }
  }
}

TEST_CASE("dropItem") {
  Matrix<Cell *> map = createCustomMap();
  MatrixRow<Entity *> entities;
  Soldier s = Soldier({2, 1}, 10, "s", 100, 100, 10, 10, 1, 1000);
  ShooterCreature sc = ShooterCreature({1, 1}, 10, "sc", 100, 100, 10, 1);
  ForagerCreature fc = ForagerCreature({1, 2}, 10, "fc", 100, 100, 10, 1000);
  entities.push_back(&s);
  entities.push_back(&sc);
  entities.push_back(&fc);
  Game g = Game(map, entities, Team::Ops);
  Controller cOps = Controller(Team::Ops, g);
  Controller cCreatures = Controller(Team::Creatures, g);
  Weapon w1 = Weapon("w1", 10, 10, 10, AmmoType::Rifle, 10);
  Weapon w2 = Weapon("w2", 10, 10, 10, AmmoType::Rifle, 10);
  Item i1 = Item("i1", 10);
  Item i2 = Item("i2", 10);
  s.hands->pickUp(w1);
  s.inventory->pickUpItem(i1);
  sc.hands->pickUp(w2);
  fc.inventory->pickUpItem(i2);

  REQUIRE_THROWS(cOps.dropItem(s, w2));
  REQUIRE_THROWS(cOps.dropItem(s, i2));
  REQUIRE_NOTHROW(cOps.dropItem(s, w1));
  REQUIRE(s.hands->weapon == nullptr);
  REQUIRE((*map[1][2]->inventory)[0] == &w1);
  REQUIRE_NOTHROW(cOps.dropItem(s, i1));
  REQUIRE((*map[1][2]->inventory)[1] == &i1);
  REQUIRE_THROWS(cOps.dropItem(s, w1));
  REQUIRE_THROWS(cOps.dropItem(s, i1));

  REQUIRE_NOTHROW(cOps.endMove());

  REQUIRE_THROWS(cCreatures.dropItem(sc, w1));
  REQUIRE_THROWS(cCreatures.dropItem(sc, i1));
  REQUIRE_THROWS(cCreatures.dropItem(sc, i2));
  REQUIRE_NOTHROW(cCreatures.dropItem(sc, w2));
  REQUIRE((*map[1][1]->inventory)[0] == &w2);

  REQUIRE_THROWS(cCreatures.dropItem(fc, w1));
  REQUIRE_THROWS(cCreatures.dropItem(fc, i1));
  REQUIRE_THROWS(cCreatures.dropItem(fc, w2));
  REQUIRE_NOTHROW(cCreatures.dropItem(fc, i2));
  REQUIRE((*map[2][1]->inventory)[0] == &i2);

  for (auto &row : map) {
    for (auto &cell : row) {
      delete cell;
    }
  }
}

TEST_CASE("pickUpItem") {
  Matrix<Cell *> map = createCustomMap();
  MatrixRow<Entity *> entities;
  Soldier s = Soldier({2, 1}, 10, "s", 100, 100, 10, 10, 1, 1000);
  ShooterCreature sc = ShooterCreature({1, 1}, 10, "sc", 100, 100, 10, 1);
  ForagerCreature fc = ForagerCreature({1, 2}, 10, "fc", 100, 100, 10, 1000);
  entities.push_back(&s);
  entities.push_back(&sc);
  entities.push_back(&fc);
  Game g = Game(map, entities, Team::Ops);
  Controller cOps = Controller(Team::Ops, g);
  Controller cCreatures = Controller(Team::Creatures, g);
  Weapon w1 = Weapon("w1", 10, 10, 10, AmmoType::Rifle, 10);
  Weapon w2 = Weapon("w2", 10, 10, 10, AmmoType::Rifle, 10);
  Item i1 = Item("i1", 10);
  Item i2 = Item("i2", 10);
  map[1][2]->inventory->pickUpItem(w1);
  map[1][2]->inventory->pickUpItem(i1);
  map[1][1]->inventory->pickUpItem(w2);
  map[2][1]->inventory->pickUpItem(i2);

  REQUIRE_THROWS(cOps.pickUpItem(s, w2));
  REQUIRE_THROWS(cOps.pickUpItem(s, i2));
  REQUIRE_NOTHROW(cOps.pickUpItem(s, w1));
  REQUIRE(s.hands->weapon == &w1);
  REQUIRE_NOTHROW(cOps.pickUpItem(s, i1));
  REQUIRE((*s.inventory)[0] == &i1);
  REQUIRE(map[1][2]->inventory->getSize() == 0);
  REQUIRE_THROWS(cOps.pickUpItem(s, w1));
  REQUIRE_THROWS(cOps.pickUpItem(s, i1));

  REQUIRE_NOTHROW(cOps.endMove());

  REQUIRE_THROWS(cCreatures.pickUpItem(sc, w1));
  REQUIRE_THROWS(cCreatures.pickUpItem(sc, i1));
  REQUIRE_THROWS(cCreatures.pickUpItem(sc, i2));
  REQUIRE_NOTHROW(cCreatures.pickUpItem(sc, w2));
  REQUIRE(sc.hands->weapon == &w2);

  REQUIRE_THROWS(cCreatures.pickUpItem(fc, w1));
  REQUIRE_THROWS(cCreatures.pickUpItem(fc, i1));
  REQUIRE_THROWS(cCreatures.pickUpItem(fc, w2));
  REQUIRE_NOTHROW(cCreatures.pickUpItem(fc, i2));
  REQUIRE((*fc.inventory)[0] == &i2);

  for (auto &row : map) {
    for (auto &cell : row) {
      delete cell;
    }
  }
}