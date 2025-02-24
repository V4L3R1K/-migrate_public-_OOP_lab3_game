#ifndef POINT_HPP
#define POINT_HPP

namespace oopLab3 {

struct Point {
  int x;
  int y;

  Point(int x, int y) : x(x), y(y) {};

  Point operator+(Point p) const { return Point(x + p.x, y + p.y); };
  Point operator-(Point p) const { return Point(x - p.x, y - p.y); };
  void operator+=(Point p) {
    x += p.x;
    y += p.y;
  };
  void operator-=(Point p) {
    x -= p.x;
    y -= p.y;
  };
};

} // namespace oopLab3

#endif // POINT_HPP