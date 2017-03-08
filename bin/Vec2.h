#ifndef VEC2_H_INCLUDED
#define VEC2_H_INCLUDED

class Vec2 {
public:
	int x, y;

	Vec2();
	Vec2(const int &x, const int &y);

	Vec2 operator+(const Vec2 &other);
};

#endif // VEC2_H_INCLUDED
