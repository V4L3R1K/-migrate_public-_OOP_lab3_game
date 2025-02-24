#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "interfaces.hpp"
#include "point.hpp"
#include <cstring>
#include <mutex>
#include <stdexcept>

/// @brief Пространство имен лабораторной работы
namespace oopLab3 {

/// @brief Класс существа
class Entity : ICanMove {
protected:
  /// @brief Mutex для доступа к данным
  mutable std::mutex mtx;

  /// @brief Позиция на карте
  Point pos;
  /// @brief Радиус обзора
  int viewRadius;
  /// @brief Отображаемое имя
  char *name;

  /// @brief Максимальное здоровье
  double maxHealth;
  /// @brief Текущий остаток здоровье
  double health;

  /// @brief Максимальное время
  int maxTime;
  /// @brief Время, затрачиваемое на перемещение
  int timeToMove;
  /// @brief Сколько времени осталось
  int time;

public:
  /// @brief Конструктор
  /// @param pos Начальная позиция
  /// @param name Отображаемое имя
  /// @param viewRadius Радиус обзора
  /// @param maxHealth Максимальное здоровье
  /// @param maxTime Максимальное время
  /// @param timeToMove Время, затрачиваемое на перемещение
  Entity(Point pos,
         int viewRadius,
         const char *name,
         double maxHealth,
         int maxTime,
         int timeToMove)
      : pos(pos), viewRadius(viewRadius), maxHealth(maxHealth),
        health(maxHealth), maxTime(maxTime), timeToMove(timeToMove), time(0) {
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
  }

  /// @brief Деструктор
  virtual ~Entity() { delete[] name; }

  /// @brief Геттер позиции
  /// @return Точка с координатами существа в пространстве
  Point getPos() const {
    std::lock_guard<std::mutex> lock(mtx);
    return pos;
  };
  /// @brief Геттер радиуса обзора
  /// @return Радиус обзора существа
  int getViewRadius() const {
    std::lock_guard<std::mutex> lock(mtx);
    return viewRadius;
  };
  /// @brief Геттер отображаемого имени
  /// @return С-Строка имени
  char *getName() const {
    std::lock_guard<std::mutex> lock(mtx);
    return name;
  };
  /// @brief Геттер максимального запаса здоровья
  /// @return Максимальное значение очков здоровья
  double getMaxHealth() const {
    std::lock_guard<std::mutex> lock(mtx);
    return maxHealth;
  };
  /// @brief Геттер текущего запаса здоровья
  /// @return Текущее значение очков здоровья
  double getHealth() const {
    std::lock_guard<std::mutex> lock(mtx);
    return health;
  };
  /// @brief Геттер максимального запаса времени
  /// @return Максимальное значение очков времени
  int getMaxTime() const {
    std::lock_guard<std::mutex> lock(mtx);
    return maxTime;
  };
  /// @brief Геттер количества очков времени, затрачиваемого на перемещение
  /// @return Количества очков времени, затрачиваемое на перемещение
  int getTimeToMove() const {
    std::lock_guard<std::mutex> lock(mtx);
    return timeToMove;
  };
  /// @brief Геттер текущего запаса времени
  /// @return Текущее значение очков времени
  int getTime() const {
    std::lock_guard<std::mutex> lock(mtx);
    return time;
  };

  /// @brief Переместить существо
  /// @param dx Сдвиг по оси X
  /// @param dy Сдвиг по оси Y
  void move(int dx, int dy) override { move(Point(dx, dy)); };
  /// @brief Переместить существо
  /// @param dPos Вектор перемещения
  void move(Point dPos) override;
  /// @brief Получить урон
  /// @param damage Количество урона
  void getDamage(double damage) {
    std::lock_guard<std::mutex> lock(mtx);
    health -= damage;
    if (health < 0) {
      health = 0;
      viewRadius = 0;
    }
  };
  /// @brief Восстановить очки времени существа
  void restoreTime() {
    std::lock_guard<std::mutex> lock(mtx);
    time = maxTime;
  }
  /// @brief Сбросить очки времени существа до нуля
  void nullifyTime() {
    std::lock_guard<std::mutex> lock(mtx);
    time = 0;
  }
  /// @brief Убрать существо с карты
  void remove() {
    std::lock_guard<std::mutex> lock(mtx);
    pos = {0, 0};
  }
};

} // namespace oopLab3

#endif // ENTITY_HPP