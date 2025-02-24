#include "view.hpp"

size_t getEntityIndexByCursor(Controller &c, Point &cursor) {
  for (size_t i = 0; i < c.game->entities->size(); i++)
    if ((*c.game->entities)[i]->getPos().x == cursor.x &&
        (*c.game->entities)[i]->getPos().y == cursor.y)
      return i;
  return c.game->entities->size();
}

void drawInventoryLine(Item &i) {
  AmmoBox *isAB = dynamic_cast<AmmoBox *>(&i);
  if (isAB)
    printw("%s (%d): %d left",
           isAB->getName(),
           isAB->getWeight(),
           isAB->getUsesLeft());
  FirstAidKit *isFAK = dynamic_cast<FirstAidKit *>(&i);
  if (isFAK)
    printw("%s (%d): %.1f*%d left time=%d",
           isFAK->getName(),
           isFAK->getWeight(),
           isFAK->getHealthPerUse(),
           isFAK->getUsesLeft(),
           isFAK->getTimeToUse());
  Weapon *isW = dynamic_cast<Weapon *>(&i);
  if (isW) {
    printw("%s (%d): dmg=%.1f time=%d",
           isW->getName(),
           isW->getWeight(),
           isW->getDamage(),
           isW->getTimeToShoot());
  }
}

void loop(Controller &c) {
  initscr();
  start_color();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  bool alive = true;
  Point cursor = {(int)(*c.game->map)[0].size() / 2,
                  (int)(*c.game->map).size() / 2};
  bool isMove = false;
  Entity *attackFrom = nullptr;
  bool pickingUp = false;
  bool dropping = false;
  int inventorySelect = -1;
  Controller cCreatures = Controller(Team::Creatures, *c.game);
  while (alive) {
    redraw(c, cursor, isMove, attackFrom, pickingUp, dropping, inventorySelect);
    size_t i = getEntityIndexByCursor(c, cursor);
    Inventory *invPtr = (*c.game->map)[cursor.y][cursor.x]->inventory;
    if (!isMove && attackFrom == nullptr && !pickingUp && !dropping &&
        c.game->getCurrentTurn() == Team::Ops)
      switch (getch()) {
      case 'Q':
        alive = false;
        break;
      case KEY_LEFT:
        cursor.x = std::max(0, cursor.x - 1);
        break;
      case KEY_RIGHT:
        cursor.x = std::min((int)(*c.game->map)[0].size() - 1, cursor.x + 1);
        break;
      case KEY_UP:
        cursor.y = std::max(0, cursor.y - 1);
        break;
      case KEY_DOWN:
        cursor.y = std::min((int)(*c.game->map).size() - 1, cursor.y + 1);
        break;
      case '\t':
        if (i == c.game->entities->size())
          cursor = (*c.game->entities)[0]->getPos();
        else
          cursor =
              (*c.game->entities)[(i + 1) % c.game->entities->size()]->getPos();
        break;
      case KEY_BTAB:
        if (i == c.game->entities->size())
          cursor = (*c.game->entities)[0]->getPos();
        else
          cursor = (*c.game->entities)[(i + c.game->entities->size() - 1) %
                                       c.game->entities->size()]
                       ->getPos();
        break;
      case 'm':
        if (i != c.game->entities->size()) {
          Soldier *isOps = dynamic_cast<Soldier *>((*c.game->entities)[i]);
          if (isOps)
            isMove = true;
        }
        break;
      case 'a':
        if (i != c.game->entities->size()) {
          Soldier *isOps = dynamic_cast<Soldier *>((*c.game->entities)[i]);
          if (isOps)
            attackFrom = isOps;
        }
        break;
      case 'r':
        if (i != c.game->entities->size())
          try {
            c.reload(*(*c.game->entities)[i]);
          } catch (std::runtime_error &e) {
          }
        break;
      case 'h':
        if (i != c.game->entities->size())
          try {
            c.heal(*(*c.game->entities)[i]);
          } catch (std::runtime_error &e) {
          }
        break;
      case '\n':
        c.endMove();
        break;
      case 'd':
        if (i != c.game->entities->size()) {
          Soldier *isOps = dynamic_cast<Soldier *>((*c.game->entities)[i]);
          if (isOps && (isOps->hands->weapon != nullptr ||
                        isOps->inventory->getSize() != 0)) {
            dropping = true;
            if (isOps->hands->weapon != nullptr)
              inventorySelect = -1;
            else
              inventorySelect = 0;
          }
        }
        break;
      case 'p':
        if (i != c.game->entities->size()) {
          Soldier *isOps = dynamic_cast<Soldier *>((*c.game->entities)[i]);
          if ((invPtr->getSize() != 0) && isOps) {
            pickingUp = true;
            inventorySelect = 0;
          }
        }
        break;
      }
    else if (isMove)
      switch (getch()) {
      case 'Q':
        alive = false;
        break;
      case KEY_LEFT:
        try {
          c.move(*(*c.game->entities)[i], {-1, 0});
          cursor.x = std::max(0, cursor.x - 1);
        } catch (std::runtime_error &e) {
          isMove = false;
        }
        break;
      case KEY_RIGHT:
        try {
          c.move(*(*c.game->entities)[i], {1, 0});
          cursor.x = std::min((int)(*c.game->map)[0].size() - 1, cursor.x + 1);
        } catch (std::runtime_error &e) {
          isMove = false;
        }
        break;
      case KEY_UP:
        try {
          c.move(*(*c.game->entities)[i], {0, -1});
          cursor.y = std::max(0, cursor.y - 1);
        } catch (std::runtime_error &e) {
          isMove = false;
        }
        break;
      case KEY_DOWN:
        try {
          c.move(*(*c.game->entities)[i], {0, 1});
          cursor.y = std::min((int)(*c.game->map).size() - 1, cursor.y + 1);
        } catch (std::runtime_error &e) {
          isMove = false;
        }
        break;
      case 'm':
        isMove = false;
        break;
      }
    else if (attackFrom != nullptr)
      switch (getch()) {
      case 'Q':
        alive = false;
        break;
      case KEY_LEFT:
        cursor.x = std::max(0, cursor.x - 1);
        break;
      case KEY_RIGHT:
        cursor.x = std::min((int)(*c.game->map)[0].size() - 1, cursor.x + 1);
        break;
      case KEY_UP:
        cursor.y = std::max(0, cursor.y - 1);
        break;
      case KEY_DOWN:
        cursor.y = std::min((int)(*c.game->map).size() - 1, cursor.y + 1);
        break;
      case '\t':
        if (i == c.game->entities->size())
          cursor = (*c.game->entities)[0]->getPos();
        else
          cursor =
              (*c.game->entities)[(i + 1) % c.game->entities->size()]->getPos();
        break;
      case KEY_BTAB:
        if (i == c.game->entities->size())
          cursor = (*c.game->entities)[0]->getPos();
        else
          cursor = (*c.game->entities)[(i + c.game->entities->size() - 1) %
                                       c.game->entities->size()]
                       ->getPos();
        break;
      case 'a':
        attackFrom = nullptr;
        break;
      case ' ':
        if (i != c.game->entities->size()) {
          try {
            c.attack(*attackFrom, *(*c.game->entities)[i]);
          } catch (std::runtime_error &e) {
            attackFrom = nullptr;
          }
        }
      }
    else if (dropping) {
      Soldier *isOps = dynamic_cast<Soldier *>((*c.game->entities)[i]);
      switch (getch()) {
      case 'Q':
        alive = false;
        break;
      case 'd':
        dropping = false;
        break;
      case KEY_UP:
        if (isOps->hands->weapon == nullptr) {
          inventorySelect += isOps->inventory->getSize() - 1;
          inventorySelect %= isOps->inventory->getSize();
        } else {
          inventorySelect += 1;
          inventorySelect -= 1;
          inventorySelect += isOps->inventory->getSize() + 1;
          inventorySelect %= (isOps->inventory->getSize() + 1);
          inventorySelect -= 1;
        }
        break;
      case KEY_DOWN:
        if (isOps->hands->weapon == nullptr) {
          inventorySelect += isOps->inventory->getSize() + 1;
          inventorySelect %= isOps->inventory->getSize();
        } else {
          inventorySelect += 1;
          inventorySelect += 1;
          inventorySelect += isOps->inventory->getSize() + 1;
          inventorySelect %= (isOps->inventory->getSize() + 1);
          inventorySelect -= 1;
        }
        break;
      case ' ':
        try {
          if (inventorySelect == -1)
            c.dropItem(*isOps, *isOps->hands->weapon);
          else
            c.dropItem(*isOps, *(*isOps->inventory)[inventorySelect]);
          dropping = false;
        } catch (std::runtime_error &e) {
        }
        break;
      }
    } else if (pickingUp)
      switch (getch()) {
      case 'Q':
        alive = false;
        break;
      case 'p':
        pickingUp = false;
        break;
      case KEY_UP:
        inventorySelect =
            (inventorySelect + invPtr->getSize() - 1) % invPtr->getSize();
        break;
      case KEY_DOWN:
        inventorySelect = (inventorySelect + 1) % invPtr->getSize();
        break;
      case ' ':
        try {
          c.pickUpItem(*(*c.game->entities)[i], *(*invPtr)[inventorySelect]);
          pickingUp = false;
        } catch (std::runtime_error &e) {
        }
        break;
      }
    else if (c.game->getCurrentTurn() == Team::Creatures)
      switch (getch()) {
      case 'Q':
        alive = false;
        break;
      case KEY_LEFT:
        cursor.x = std::max(0, cursor.x - 1);
        break;
      case KEY_RIGHT:
        cursor.x = std::min((int)(*c.game->map)[0].size() - 1, cursor.x + 1);
        break;
      case KEY_UP:
        cursor.y = std::max(0, cursor.y - 1);
        break;
      case KEY_DOWN:
        cursor.y = std::min((int)(*c.game->map).size() - 1, cursor.y + 1);
        break;
      case '\t':
        if (i == c.game->entities->size())
          cursor = (*c.game->entities)[0]->getPos();
        else
          cursor =
              (*c.game->entities)[(i + 1) % c.game->entities->size()]->getPos();
        break;
      case KEY_BTAB:
        if (i == c.game->entities->size())
          cursor = (*c.game->entities)[0]->getPos();
        else
          cursor = (*c.game->entities)[(i + c.game->entities->size() - 1) %
                                       c.game->entities->size()]
                       ->getPos();
        break;
      case ' ':
        tick(cCreatures, true);
        break;
      case '\n':
        tickTillEnd(cCreatures, true);
        break;
      }
  }
  endwin();
}

void redraw(Controller &c,
            Point &cursor,
            bool isMove,
            Entity *attackFrom,
            bool pickingUp,
            bool dropping,
            int inventorySelect) {
  move(0, 0);
  clear();
  drawMap(c);
  drawEntities(c);
  drawInfo(c, cursor, isMove, attackFrom, pickingUp, dropping, inventorySelect);
  refresh();
}

void drawMap(Controller &c) {
  Matrix<Cell *> *mapPtr = c.game->map;
  init_pair(1, COLOR_CYAN, COLOR_BLACK);   // glass
  init_pair(2, COLOR_BLACK, COLOR_BLACK);  // wall
  init_pair(3, COLOR_BLACK, COLOR_BLACK);  // floor
  init_pair(4, COLOR_BLACK, COLOR_BLACK);  // wall breakable
  init_pair(5, COLOR_BLACK, COLOR_YELLOW); // storage
  init_pair(10, COLOR_WHITE, COLOR_BLACK); // floor with loot
  for (size_t y = 0; y < mapPtr->size(); y++)
    for (size_t x = 0; x < ((*mapPtr)[0]).size(); x++) {
      int color_pair = 0;
      char symbol = ' ';
      switch ((*mapPtr)[y][x]->getCellType()) {
      case CellType::Glass:
        color_pair = 1;
        symbol = '#';
        break;
      case CellType::Wall:
        color_pair = 2;
        symbol = '#';
        break;
      case CellType::Floor:
        if ((*mapPtr)[y][x]->inventory->getSize() == 0)
          color_pair = 3;
        else
          color_pair = 10;
        symbol = '.';
        break;
      case CellType::WallBreakable:
        color_pair = 4;
        symbol = '*';
        break;
      case CellType::Storage:
        color_pair = 5;
        symbol = '.';
        break;
      }
      attron(COLOR_PAIR(color_pair));
      mvaddch(y, 1 + x * 2, symbol);
      attroff(COLOR_PAIR(color_pair));
    }
}

void drawEntities(Controller &c) {
  MatrixRow<Entity *> *entitiesPtr = c.game->entities;
  init_pair(6, COLOR_GREEN, COLOR_BLACK); // ops
  init_pair(7, COLOR_RED, COLOR_BLACK);   // creatures
  for (size_t i = 0; i < entitiesPtr->size(); i++) {
    Soldier *isOps = dynamic_cast<Soldier *>((*entitiesPtr)[i]);
    ForagerCreature *isF = dynamic_cast<ForagerCreature *>((*entitiesPtr)[i]);
    ShooterCreature *isS = dynamic_cast<ShooterCreature *>((*entitiesPtr)[i]);
    WildCreature *isW = dynamic_cast<WildCreature *>((*entitiesPtr)[i]);
    if (isOps) {
      attron(COLOR_PAIR(6));
      mvaddch((*entitiesPtr)[i]->getPos().y,
              (*entitiesPtr)[i]->getPos().x * 2 + 1,
              'S');
      attroff(COLOR_PAIR(6));
    }
    if (isF) {
      attron(COLOR_PAIR(7));
      mvaddch((*entitiesPtr)[i]->getPos().y,
              (*entitiesPtr)[i]->getPos().x * 2 + 1,
              'F');
      attroff(COLOR_PAIR(7));
    }
    if (isS) {
      attron(COLOR_PAIR(7));
      mvaddch((*entitiesPtr)[i]->getPos().y,
              (*entitiesPtr)[i]->getPos().x * 2 + 1,
              'S');
      attroff(COLOR_PAIR(7));
    }
    if (isW) {
      attron(COLOR_PAIR(7));
      mvaddch((*entitiesPtr)[i]->getPos().y,
              (*entitiesPtr)[i]->getPos().x * 2 + 1,
              'W');
      attroff(COLOR_PAIR(7));
    }
  }
}

void drawInfo(Controller &c,
              Point &cursor,
              bool isMove,
              Entity *attackFrom,
              bool pickingUp,
              bool dropping,
              int inventorySelect) {
  mvaddch(cursor.y, cursor.x * 2, '[');
  mvaddch(cursor.y, cursor.x * 2 + 2, ']');

  move(0, (*c.game->map)[0].size() * 2 + 1);
  if (c.game->getCurrentTurn() == Team::Ops)
    printw("Your turn");
  else
    printw("Opponent's turn");

  move(2, (*c.game->map)[0].size() * 2 + 1);
  printw("Cursor: ");
  move(2, (*c.game->map)[0].size() * 2 + 9);
  switch ((*c.game->map)[cursor.y][cursor.x]->getCellType()) {
  case CellType::Glass:
    printw("glass");
    break;
  case CellType::Wall:
    printw("wall");
    break;
  case CellType::Floor:
    printw("floor");
    break;
  case CellType::WallBreakable:
    printw("wallBreakable");
    break;
  case CellType::Storage:
    printw("storage");
    break;
  }

  size_t i = getEntityIndexByCursor(c, cursor);
  Inventory *invPtrCell = (*c.game->map)[cursor.y][cursor.x]->inventory;
  if (i != c.game->entities->size()) {
    move(3, (*c.game->map)[0].size() * 2 + 1);
    if (isMove)
      printw("%s (walking)", (*c.game->entities)[i]->getName());
    else if ((*c.game->entities)[i] == attackFrom)
      printw("%s (attacking)", (*c.game->entities)[i]->getName());
    else if (pickingUp)
      printw("%s (picking up)", (*c.game->entities)[i]->getName());
    else if (dropping)
      printw("%s (dropping)", (*c.game->entities)[i]->getName());
    else
      printw((*c.game->entities)[i]->getName());

    move(4, (*c.game->map)[0].size() * 2 + 1);
    printw("Health: %.1f / %.1f",
           (*c.game->entities)[i]->getHealth(),
           (*c.game->entities)[i]->getMaxHealth());
    move(5, (*c.game->map)[0].size() * 2 + 1);
    printw("Time: %d / %d",
           (*c.game->entities)[i]->getTime(),
           (*c.game->entities)[i]->getMaxTime());
    move(6, (*c.game->map)[0].size() * 2 + 1);
    printw("Time to move: %d (%d moves)",
           (*c.game->entities)[i]->getTimeToMove(),
           (*c.game->entities)[i]->getMaxTime() /
               (*c.game->entities)[i]->getTimeToMove());

    Soldier *isOps = dynamic_cast<Soldier *>((*c.game->entities)[i]);
    ShooterCreature *isS =
        dynamic_cast<ShooterCreature *>((*c.game->entities)[i]);
    ForagerCreature *isF =
        dynamic_cast<ForagerCreature *>((*c.game->entities)[i]);
    WildCreature *isW = dynamic_cast<WildCreature *>((*c.game->entities)[i]);

    if (isOps) {
      move(10 + invPtrCell->getSize(), (*c.game->map)[0].size() * 2 + 1);
      if (isOps->hands->weapon != nullptr) {
        printw("Hands (%d): %s",
               isOps->hands->weapon->getWeight(),
               isOps->hands->weapon->getName());
        move(11 + invPtrCell->getSize(), (*c.game->map)[0].size() * 2 + 1);
        printw("Time to shoot: %d", isOps->hands->weapon->getTimeToShoot());
        move(12 + invPtrCell->getSize(), (*c.game->map)[0].size() * 2 + 1);
        printw("Ammo: %d / %d",
               isOps->hands->weapon->getAmmoLeft(),
               isOps->hands->weapon->getMaxAmmo());
        move(13 + invPtrCell->getSize(), (*c.game->map)[0].size() * 2 + 1);
        printw("Damage: %.1f", isOps->hands->weapon->getDamage());
      } else {
        printw("Hands (0): no weapon");
      }
      move(14 + invPtrCell->getSize(), (*c.game->map)[0].size() * 2 + 1);
      printw("Soldier inventory (%d / %d):",
             isOps->inventory->getCurrentWeight(),
             isOps->inventory->getMaxWeight());
      for (int i = 0; i < isOps->inventory->getSize(); i++) {
        move(15 + i + invPtrCell->getSize(), (*c.game->map)[0].size() * 2 + 1);
        drawInventoryLine(*(*isOps->inventory)[i]);
      }
    }

    if (isS) {
      move(10 + invPtrCell->getSize(), (*c.game->map)[0].size() * 2 + 1);
      if (isS->hands->weapon != nullptr) {
        printw("Hands (%d): %s",
               isS->hands->weapon->getWeight(),
               isS->hands->weapon->getName());
        move(11 + invPtrCell->getSize(), (*c.game->map)[0].size() * 2 + 1);
        printw("Time to shoot: %d", isS->hands->weapon->getTimeToShoot());
        move(12 + invPtrCell->getSize(), (*c.game->map)[0].size() * 2 + 1);
        printw("Ammo: %d / %d",
               isS->hands->weapon->getAmmoLeft(),
               isS->hands->weapon->getMaxAmmo());
        move(13 + invPtrCell->getSize(), (*c.game->map)[0].size() * 2 + 1);
        printw("Damage: %.1f", isS->hands->weapon->getDamage());
      } else {
        printw("Hands (0): no weapon");
      }
    }

    if (isF) {
      printw("Forager inventory (%d / %d):",
             isF->inventory->getCurrentWeight(),
             isF->inventory->getMaxWeight());
      for (int i = 0; i < isF->inventory->getSize(); i++) {
        move(15 + i + invPtrCell->getSize(), (*c.game->map)[0].size() * 2 + 1);
        drawInventoryLine(*(*isF->inventory)[i]);
      }
    }

    if (isW) {
      move(11 + invPtrCell->getSize(), (*c.game->map)[0].size() * 2 + 1);
      printw("Time to punch: %d", isW->getTimeToPunch());
      move(13 + invPtrCell->getSize(), (*c.game->map)[0].size() * 2 + 1);
      printw("Damage: %.1f", isW->getMeleeDamage());
    }
  }

  move(8, (*c.game->map)[0].size() * 2 + 1);
  if (invPtrCell->getSize() != 0) {
    printw("Cell inventory (%d):", invPtrCell->getCurrentWeight());
    for (int i = 0; i < invPtrCell->getSize(); i++) {
      move(9 + i, (*c.game->map)[0].size() * 2 + 1);
      drawInventoryLine(*(*invPtrCell)[i]);
    }
  } else
    printw("Cell inventory (0): empty");

  if (dropping) {
    if (inventorySelect == -1)
      mvaddch(10 + invPtrCell->getSize(), (*c.game->map)[0].size() * 2, '>');
    else
      mvaddch(15 + invPtrCell->getSize() + inventorySelect,
              (*c.game->map)[0].size() * 2,
              '>');
  }

  if (pickingUp) {
    mvaddch(9 + inventorySelect, (*c.game->map)[0].size() * 2, '>');
  }
}