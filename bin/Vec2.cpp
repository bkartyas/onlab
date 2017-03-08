#include "Vec2.h"

Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(const int &x, const int &y) : x(x), y(y) {}

Vec2 Vec2::operator+(const Vec2 &other) {
	return Vec2(other.x+x, other.y+y);
}

