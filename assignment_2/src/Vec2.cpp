#include <math.h>
#include <iostream>
#include "Vec2.h"



Vec2::Vec2() {
	x = y = 0;
};
Vec2::Vec2(float xin, float yin) :
	x(xin),
	y(yin)
{
};

bool Vec2::operator==(const Vec2& rhs) const {
	return (rhs.x == x && rhs.y == y);
};
bool Vec2::operator!=(const Vec2& rhs) const {
	return !(rhs.x == x && rhs.y == y);
};

Vec2 Vec2::operator+(const Vec2& rhs) const {
	return Vec2(x + rhs.x, y + rhs.y);
};
Vec2 Vec2::operator-(const Vec2& rhs) const {
	return Vec2(x - rhs.x, y - rhs.y);
};
Vec2 Vec2::operator/(const float val) const {
	return Vec2(x / val, y / val);
};
Vec2 Vec2::operator*(const float val) const {
	return Vec2(x * val, y * val);
};

void Vec2::operator+=(const Vec2& rhs) {
	x += rhs.x;
	y += rhs.y;
};
void Vec2::operator-=(const Vec2& rhs) {
	x -= rhs.x;
	y -= rhs.y;
};
void Vec2::operator*=(const float val) {
	x *= val;
	y *= val;
};
void Vec2::operator/=(const float val) {
	x /= val;
	y /= val;
};

float Vec2::dist(const Vec2& rhs) const {
	return sqrtf((x - rhs.x) * (x - rhs.x) + (y - rhs.y)* (y - rhs.y));
};
float Vec2::length() const {
	return sqrt(x * x + y * y);
};
void Vec2::normalize() {
	float len = length();
	*this /= len;
};
void Vec2::print() {
	std::cout << x << " " << y << "\n";
}
