#ifndef VIEW_HPP
#define VIEW_HPP

#include "../controller/controller.hpp"
#include "../controller/creaturesAI.hpp"
#include "../model/ammoBox.hpp"
#include "../model/ammoType.hpp"
#include "../model/cell.hpp"
#include "../model/entity.hpp"
#include "../model/firstAidKit.hpp"
#include "../model/foragerCreature.hpp"
#include "../model/game.hpp"
#include "../model/hands.hpp"
#include "../model/interfaces.hpp"
#include "../model/inventory.hpp"
#include "../model/item.hpp"
#include "../model/matrix.hpp"
#include "../model/point.hpp"
#include "../model/shooterCreature.hpp"
#include "../model/soldier.hpp"
#include "../model/team.hpp"
#include "../model/weapon.hpp"
#include "../model/wildCreature.hpp"

#include <ncurses.h>

using namespace oopLab3;

size_t getEntityIndexByCursor(Controller &c, Point &cursor);
void loop(Controller &c);
void redraw(Controller &c,
            Point &cursor,
            bool isMove,
            Entity *attackFrom,
            bool pickingUp,
            bool dropping,
            int inventorySelect);
void drawMap(Controller &c);
void drawEntities(Controller &c);
void drawInfo(Controller &c,
              Point &cursor,
              bool isMove,
              Entity *attackFrom,
              bool pickingUp,
              bool dropping,
              int inventorySelect);
void drawInventoryLine(Item &i);

#endif