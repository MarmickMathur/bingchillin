#pragma once
#include "Vec2.h"
#include <SFML/Graphics.hpp>

class cTransform {
public:
	Vec2 pos = {0.0f, 0.0f};
	Vec2 velocity = {0.0f , 0.0f};
	float angle;

	cTransform(const Vec2 posIn, const Vec2 velocity, const float angle);
};

class cShape {
public:
	sf::CircleShape circle;
	cShape(int sides , float radius,const sf::Color& fill , const sf::Color& outline,const float thickness);
};

class cCollision {
public:
	float collisionRadius = 0;
	cCollision(const float radius);
};

class cScore {
public:
	int score = 0;
	cScore(int s);
};

class cLifeSpan {
public:
	int reamining = 0;
	int total = 0;
	cLifeSpan(int total);
};

class cInput {
public:
	bool up = 0;
	bool down = 0;
	bool right = 0;
	bool left = 0;
	bool leftMouse = 0;
	bool rightMouse = 0;
	cInput();
};