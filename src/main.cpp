#include "view/view.hpp"

Matrix<Cell *> initMap(int w, int h, const char *mapStr) {
  Matrix<Cell *> map = {};
  for (int y = 0; y < w; y++) {
    map.push_back({});
    for (int x = 0; x < h; x++) {
      switch (mapStr[y * w + x]) {
      case '#':
        map[map.size() - 1].push_back(new Cell(CellType::Wall));
        break;
      case 'G':
        map[map.size() - 1].push_back(new Cell(CellType::Glass));
        break;
      case 'W':
        map[map.size() - 1].push_back(new Cell(CellType::WallBreakable));
        break;
      case ' ':
        map[map.size() - 1].push_back(new Cell(CellType::Floor));
        break;
      case 'S':
        map[map.size() - 1].push_back(new Cell(CellType::Storage));
        break;
      }
    }
  }
  return map;
}
int main() {
  Matrix<Cell *> map = initMap(32,
                               32,
                               "################################"
                               "#     #    #                   #"
                               "#                #             #"
                               "#                #      ########"
                               "#                #         #   #"
                               "#     #WS    ##W#####      #   #"
                               "#                   #      #   #"
                               "#  S           S    #      #   #"
                               "#                   #          #"
                               "#WW  W#    ###############W#####"
                               "#          #                   #"
                               "#                              #"
                               "#                   #          #"
                               "#          #                   #"
                               "#          #                   #"
                               "#GGGGGGGGGG#####  ###          #"
                               "#                   #          #"
                               "#                   #          #"
                               "#                   ####   #####"
                               "#  S      S         #          #"
                               "#         #         #          #"
                               "#    S    #         #          #"
                               "######    #################WWW##"
                               "#    #    #         W          #"
                               "#    #    #         W          #"
                               "#              #         #     #"
                               "#    #    #    #         #     #"
                               "##GG###  ###GG###GG###GG###GG###"
                               "#                              #"
                               "#                              #"
                               "#                              #"
                               "################################");
  MatrixRow<Entity *> entities;
  Soldier s1 = Soldier({29, 29}, 10, "Beff Jesos", 100, 100, 5, 15, 0.7, 50);
  Weapon w1 = Weapon("amazing rifle", 20, 10, 5, AmmoType::Rifle, 15);
  s1.hands->pickUp(w1);
  Soldier s2 = Soldier({27, 29}, 10, "Melon Lusk", 150, 100, 4, 10, 0.5, 50);
  Weapon w2 = Weapon("tesla coil pistol", 15, 8, 12, AmmoType::Pistol, 10);
  s2.hands->pickUp(w2);
  Soldier s3 = Soldier({23, 29}, 10, "Zark Muckerberg", 75, 750, 3, 5, 1, 15);
  Weapon w3 =
      Weapon("face look crusher", 10, 6, 15, AmmoType::HeavyRifle, 200, 150);
  s3.hands->pickUp(w3);
  ForagerCreature fc1 =
      ForagerCreature({15, 3}, 10, "Vasya Sapog", 90, 50, 2, 1000);
  ForagerCreature fc2 =
      ForagerCreature({15, 20}, 10, "Jenya Dudka", 80, 40, 2, 800);
  ShooterCreature sc1 =
      ShooterCreature({4, 19}, 8, "Pasha Gniloy", 100, 70, 5, 0.5);
  Weapon w4 = Weapon("basic revolver", 10, 15, 15, AmmoType::Revolver, 6, 4);
  sc1.hands->pickUp(w4);
  ShooterCreature sc2 =
      ShooterCreature({4, 11}, 8, "Misha Pahan", 200, 60, 6, 0.7);
  Weapon w5 = Weapon("buckshot9000", 120, 25, 5, AmmoType::Shotgun, 8);
  map[5][5]->inventory->pickUpItem(w5);
  Weapon w6 = Weapon("w6", 120, 25, 5, AmmoType::Pistol, 8);
  map[8][7]->inventory->pickUpItem(w6);
  Weapon w7 = Weapon("w6", 120, 25, 5, AmmoType::Pistol, 8);
  map[6][5]->inventory->pickUpItem(w7);
  WildCreature wc1 =
      WildCreature({13, 25}, 5, "Pavlik Motyga", 60, 100, 4, 4, 5, 0.5);
  WildCreature wc2 =
      WildCreature({26, 12}, 5, "Petr Zhelezo", 150, 100, 10, 10, 15, 0.75);
  entities.push_back(&s1);
  entities.push_back(&s2);
  entities.push_back(&s3);
  entities.push_back(&fc1);
  entities.push_back(&fc2);
  entities.push_back(&sc1);
  entities.push_back(&sc2);
  entities.push_back(&wc1);
  entities.push_back(&wc2);
  Game g = Game(map, entities, Team::Ops);
  Controller c = Controller(Team::Ops, g);

  FirstAidKit fak1 = FirstAidKit("tabletos", 5, 3, 10, 2);
  s1.inventory->pickUpItem(fak1);
  AmmoBox ab1 = AmmoBox("rifle ammo", 10, AmmoType::Rifle, 10, 2);
  s1.inventory->pickUpItem(ab1);

  loop(c);

  for (auto &row : *g.map)
    for (auto &cell : row)
      delete cell;
}