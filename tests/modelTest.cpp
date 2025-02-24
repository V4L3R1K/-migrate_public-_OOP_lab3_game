#define CATCH_CONFIG_MAIN

#include "../src/model/ammoBox.hpp"
#include "../src/model/ammoType.hpp"
#include "../src/model/cell.hpp"
#include "../src/model/entity.hpp"
#include "../src/model/firstAidKit.hpp"
#include "../src/model/foragerCreature.hpp"
#include "../src/model/game.hpp"
#include "../src/model/hands.hpp"
#include "../src/model/interfaces.hpp"
#include "../src/model/inventory.hpp"
#include "../src/model/item.hpp"
#include "../src/model/matrix.hpp"
#include "../src/model/point.hpp"
#include "../src/model/shooterCreature.hpp"
#include "../src/model/soldier.hpp"
#include "../src/model/team.hpp"
#include "../src/model/weapon.hpp"
#include "../src/model/wildCreature.hpp"

#include <catch2/catch_all.hpp>
#include <string>

using namespace oopLab3;

TEST_CASE("items") {
  SECTION("ammoBox") {
    AmmoBox ab("ab", 120, AmmoType::Rifle, 10, 5);
    REQUIRE(ab.getAmmoPerUse() == 10);
    REQUIRE(ab.getAmmoType() == AmmoType::Rifle);
    REQUIRE(strcmp(ab.getName(), "ab") == 0);
    REQUIRE(ab.getUsesLeft() == 5);
    REQUIRE(ab.getWeight() == 120);

    REQUIRE_NOTHROW(ab.use());
    REQUIRE(ab.getUsesLeft() == 4);
    REQUIRE_NOTHROW(ab.use());
    REQUIRE_NOTHROW(ab.use());
    REQUIRE_NOTHROW(ab.use());
    REQUIRE(ab.getUsesLeft() == 1);
    REQUIRE_NOTHROW(ab.use());
    REQUIRE(ab.getUsesLeft() == 0);
    REQUIRE_THROWS(ab.use());
    REQUIRE(ab.getUsesLeft() == 0);
  }

  SECTION("firstAidKit") {
    FirstAidKit fak("fak", 100, 10, 20, 3);

    REQUIRE(fak.getHealthPerUse() == 20);
    REQUIRE(fak.getTimeToUse() == 10);
    REQUIRE(fak.getUsesLeft() == 3);
    REQUIRE_NOTHROW(fak.use());
    REQUIRE(fak.getUsesLeft() == 2);
    REQUIRE_NOTHROW(fak.use());
    REQUIRE_NOTHROW(fak.use());
    REQUIRE(fak.getUsesLeft() == 0);
    REQUIRE_THROWS(fak.use());
    REQUIRE(fak.getUsesLeft() == 0);
  }

  SECTION("weapon") {
    Weapon w1 = Weapon("w", 100, 100, 10, AmmoType::Rifle, 15);
    REQUIRE(w1.getDamage() == 100);
    REQUIRE(w1.getTimeToShoot() == 10);
    REQUIRE(w1.getAmmoType() == AmmoType::Rifle);
    REQUIRE(w1.getMaxAmmo() == 15);
    REQUIRE(w1.getAmmoLeft() == w1.getMaxAmmo());

    Weapon w2 = Weapon("w", 100, 100, 10, AmmoType::Revolver, 6, 2);
    REQUIRE_NOTHROW(w2.shoot());
    REQUIRE(w2.getAmmoLeft() == 1);
    REQUIRE_NOTHROW(w2.shoot());
    REQUIRE(w2.getAmmoLeft() == 0);
    REQUIRE_THROWS(w2.shoot());

    REQUIRE_NOTHROW(w2.reload());
    REQUIRE(w2.getAmmoLeft() == w2.getMaxAmmo());
  }
}

TEST_CASE("cell") {
  SECTION("glass") {
    Cell c1(CellType::Glass);
    REQUIRE(c1.getCellType() == CellType::Glass);
    REQUIRE_NOTHROW(c1.getShot());
    REQUIRE(c1.getCellType() == CellType::Floor);
  }

  SECTION("wallBreakable") {
    Inventory *i = new Inventory(-1);
    Cell c2(CellType::WallBreakable, *i);
    REQUIRE_NOTHROW(c2.getShot());
    REQUIRE(c2.getCellType() == CellType::Floor);
  }

  SECTION("unbreakable") {
    Cell c3(CellType::Wall);
    REQUIRE_NOTHROW(c3.getShot());
    REQUIRE(c3.getCellType() == CellType::Wall);
    REQUIRE(c3.inventory->getMaxWeight() == -1);
  }
}

TEST_CASE("entity") {
  SECTION("constructor") {
    Entity e({1, 2}, 4, "ent", 100, 5, 1);

    REQUIRE(e.getPos().x == 1);
    REQUIRE(e.getPos().y == 2);
    REQUIRE(e.getViewRadius() == 4);
    REQUIRE(strcmp(e.getName(), "ent") == 0);
    REQUIRE(e.getMaxHealth() == 100);
    REQUIRE(e.getHealth() == 100);
    REQUIRE(e.getMaxTime() == 5);
    REQUIRE(e.getTimeToMove() == 1);
    REQUIRE(e.getTime() == 0);
  }

  SECTION("getDamage") {
    Entity e({1, 2}, 4, "ent", 100, 5, 1);

    REQUIRE_NOTHROW(e.getDamage(10));
    REQUIRE(e.getHealth() == 90);

    REQUIRE_NOTHROW(e.getDamage(100));
    REQUIRE(e.getHealth() == 0);
    REQUIRE(e.getViewRadius() == 0);
  }

  SECTION("move") {
    Entity e({1, 2}, 4, "ent", 100, 1, 1);

    REQUIRE_THROWS(e.move(1, 1));
    REQUIRE(e.getPos().x == 1);
    REQUIRE(e.getPos().y == 2);

    REQUIRE_NOTHROW(e.restoreTime());
    REQUIRE(e.getTime() == e.getMaxTime());
    REQUIRE_NOTHROW(e.move(1, 1));
    REQUIRE(e.getPos().x == 2);
    REQUIRE(e.getPos().y == 3);

    REQUIRE(e.getTime() == 0);
    REQUIRE_THROWS(e.move(1, 1));
    REQUIRE(e.getPos().x == 2);
    REQUIRE(e.getPos().y == 3);

    REQUIRE_NOTHROW(e.restoreTime());
    REQUIRE_NOTHROW(e.getDamage(100));
    REQUIRE_THROWS(e.move(1, 1));
    REQUIRE(e.getPos().x == 2);
    REQUIRE(e.getPos().y == 3);
  }

  SECTION("foragerCreature") {
    ForagerCreature fc({0, 0}, 10, "123", 100, 10, 10, 100);
    REQUIRE(fc.inventory->getMaxWeight() == 100);

    Inventory *i = new Inventory(1000);
    ForagerCreature fc2({0, 0}, 10, "123", 100, 10, 10, *i);
    REQUIRE(fc2.inventory->getMaxWeight() == 1000);
  }

  SECTION("soldier") {
    SECTION("firstAidKit") {
      Soldier s1({0, 0}, 10, "123", 100, 100, 10, 10, 10, 10);
      REQUIRE(s1.getTimeToReload() == 10);

      s1.getDamage(10);
      REQUIRE(s1.getMaxHealth() == 100);
      REQUIRE(s1.getHealth() == 90);

      REQUIRE_THROWS(s1.heal());

      FirstAidKit noUsesFAK = FirstAidKit("123", 5, 10, 5, 0);
      REQUIRE_NOTHROW(s1.restoreTime());
      REQUIRE_NOTHROW(s1.inventory->pickUpItem(noUsesFAK));
      REQUIRE_THROWS(s1.heal());
      REQUIRE(s1.getHealth() == 90);
      REQUIRE(s1.getTime() == 100);

      FirstAidKit largeTimeFAK = FirstAidKit("123", 4, 1000000, 5, 1);
      REQUIRE_NOTHROW(s1.inventory->pickUpItem(largeTimeFAK));
      REQUIRE_THROWS(s1.heal());
      REQUIRE(s1.getHealth() == 90);
      REQUIRE(s1.getTime() == 100);

      FirstAidKit noSpaceFAK = FirstAidKit("123", 4, 1000000, 5, 1);
      REQUIRE_THROWS(s1.inventory->pickUpItem(noSpaceFAK));

      REQUIRE_NOTHROW(s1.inventory->dropItem(0));
      REQUIRE_NOTHROW(s1.inventory->dropItem(0));
      REQUIRE(s1.inventory->getSize() == 0);

      Item notFAK = Item("item", 1);
      REQUIRE_NOTHROW(s1.inventory->pickUpItem(notFAK));

      FirstAidKit FAK = FirstAidKit("fak", 1, 10, 5, 1);
      REQUIRE_NOTHROW(s1.inventory->pickUpItem(FAK));
      REQUIRE_NOTHROW(s1.heal());
      REQUIRE(s1.getHealth() == 95);
      REQUIRE(s1.getTime() == 90);

      s1.getDamage(1000);
      REQUIRE_THROWS(s1.heal());
    }

    SECTION("shoot") {
      Soldier s = Soldier({0, 0}, 10, "s", 100, 100, 10, 10, 0.9, 10000);
      REQUIRE_THROWS(s.shoot());
      Weapon w = Weapon("w", 10, 100, 89, AmmoType::Rifle, 10, 1);
      REQUIRE_NOTHROW(s.hands->pickUp(w));
      REQUIRE_NOTHROW(s.restoreTime());
      REQUIRE_NOTHROW(s.shoot());
      REQUIRE(s.getTime() == 11);

      Weapon w2 = Weapon("w", 10, 100, 12, AmmoType::Rifle, 10, 1);
      REQUIRE_THROWS(s.hands->pickUp(w2));
      REQUIRE_NOTHROW(s.hands->drop());
      REQUIRE_NOTHROW(s.hands->pickUp(w2));
      REQUIRE_THROWS(s.shoot());
      REQUIRE(s.getTime() == 11);

      s.getDamage(1000);
      REQUIRE_THROWS(s.shoot());
    }

    SECTION("reload") {
      Weapon w = Weapon("w", 10, 100, 10, AmmoType::Rifle, 10, 5);
      AmmoBox ab = AmmoBox("ab", 10, AmmoType::Rifle, 10, 0);
      Soldier s = Soldier({0, 0}, 10, "s", 100, 100, 10, 10, 0.9, 1000);
      Item item = Item("item", 10);
      REQUIRE_NOTHROW(s.inventory->pickUpItem(item));
      AmmoBox abR = AmmoBox("ab", 10, AmmoType::Revolver, 10, 1);
      REQUIRE_NOTHROW(s.inventory->pickUpItem(abR));

      REQUIRE_THROWS(s.reload());

      REQUIRE_NOTHROW(s.inventory->pickUpItem(ab));

      REQUIRE_THROWS(s.reload());

      REQUIRE_NOTHROW(s.hands->pickUp(w));

      REQUIRE_THROWS(s.reload());

      REQUIRE_NOTHROW(s.restoreTime());

      REQUIRE_THROWS(s.reload());

      AmmoBox ab1 = AmmoBox("ab", 10, AmmoType::Rifle, 10, 1);
      REQUIRE_NOTHROW(s.inventory->pickUpItem(ab1));

      REQUIRE_NOTHROW(s.reload());
      REQUIRE(s.getTime() == 90);
      REQUIRE(w.getAmmoLeft() == w.getMaxAmmo());

      s.getDamage(1000);
      REQUIRE_THROWS(s.reload());
    }
  }

  SECTION("shooterCreature") {
    ShooterCreature sc1 = ShooterCreature({0, 0}, 10, "sc", 100, 100, 10, 0.9);
    REQUIRE_THROWS(sc1.shoot());
    Weapon w = Weapon("w", 10, 100, 89, AmmoType::Rifle, 10, 1);
    REQUIRE_NOTHROW(sc1.hands->pickUp(w));
    REQUIRE_NOTHROW(sc1.restoreTime());
    REQUIRE_NOTHROW(sc1.shoot());
    REQUIRE(sc1.getTime() == 11);

    Weapon w2 = Weapon("w", 10, 100, 12, AmmoType::Rifle, 10, 1);
    REQUIRE_THROWS(sc1.hands->pickUp(w2));
    REQUIRE_NOTHROW(sc1.hands->drop());
    REQUIRE_NOTHROW(sc1.hands->pickUp(w2));
    REQUIRE_THROWS(sc1.shoot());
    REQUIRE(sc1.getTime() == 11);

    sc1.getDamage(1000);
    REQUIRE_THROWS(sc1.shoot());

    Hands *h = new Hands(0.9);
    ShooterCreature sc2 = ShooterCreature({0, 0}, 10, "sc", 100, 100, 10, *h);
  }

  SECTION("wildCreature") {
    WildCreature wc = WildCreature({0, 0}, 10, "wc", 100, 100, 10, 10, 100, 1);
    REQUIRE_THROWS(wc.punch());
    REQUIRE_NOTHROW(wc.restoreTime());
    REQUIRE_NOTHROW(wc.punch());
    wc.getDamage(1000);
    REQUIRE_THROWS(wc.punch());
    REQUIRE_NOTHROW(wc.getDamage(100));
    REQUIRE(wc.getMeleeDamage() == 100);
    REQUIRE(wc.getMeleeAccuracy() == 1);
    REQUIRE(wc.getTimeToPunch() == 10);
    REQUIRE(wc.getTime() == 90);
  }
}

TEST_CASE("inventory") {
  Inventory i(10);
  REQUIRE(i.getCurrentWeight() == 0);
  REQUIRE(i.getMaxWeight() == 10);
  REQUIRE(i.getSize() == 0);

  Item item1 = Item("1", 5);
  REQUIRE_NOTHROW(i.pickUpItem(item1));
  REQUIRE(i.getCurrentWeight() == 5);
  REQUIRE(i.getSize() == 1);

  Item item2 = Item("2", 10);
  REQUIRE_THROWS(i.pickUpItem(item2));

  Item item3 = Item("3", 5);
  REQUIRE_NOTHROW(i.pickUpItem(item3));
  REQUIRE(i.getCurrentWeight() == 10);
  REQUIRE(i.getSize() == 2);

  REQUIRE(i[1] == &item3);

  REQUIRE_NOTHROW(i.dropItem(0));
  REQUIRE(i[0] == &item3);
}

TEST_CASE("point") {
  Point p1(1, 2);
  Point p2 = {3, 4};

  REQUIRE((p1 + p2).x == 4);
  REQUIRE((p1 + p2).y == 6);

  REQUIRE((p1 - p2).x == -2);
  REQUIRE((p1 - p2).y == -2);

  p1 += {1, 1};
  REQUIRE(p1.x == 2);
  REQUIRE(p1.y == 3);

  p1 -= {-10, 11};
  REQUIRE(p1.x == 12);
  REQUIRE(p1.y == -8);
}

TEST_CASE("game") {
  Matrix<Cell *> map;
  MatrixRow<Entity *> entities;
  Game g = Game(map, entities, Team::Ops);

  REQUIRE(g.getCurrentTurn() == Team::Ops);
  REQUIRE_NOTHROW(g.swapTurn());
  REQUIRE(g.getCurrentTurn() == Team::Creatures);
  REQUIRE_NOTHROW(g.swapTurn());
  REQUIRE(g.getCurrentTurn() == Team::Ops);

  Soldier s = Soldier({0, 0}, 10, "s", 100, 100, 10, 10, 0.9, 1000);
  ForagerCreature f = ForagerCreature({0, 0}, 10, "f", 100, 100, 10, 1000);
  entities.push_back(&s);
  entities.push_back(&f);
  REQUIRE_THROWS(g.whoWon());
  s.getDamage(1000);
  REQUIRE(g.whoWon() == Team::Creatures);
  f.getDamage(1000);
  REQUIRE(g.whoWon() == Team::Ops);
}