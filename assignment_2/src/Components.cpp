#include "Vec2.h"
#include <SFML/Graphics.hpp>
#include "Components.h"

cTransform::cTransform(const Vec2 posIn, const Vec2 velocity, const float angle) :
	pos(posIn),
	velocity(velocity),
	angle(angle)
{
}

cShape::cShape(int sides, float radius, const sf::Color& fill, const sf::Color& outline, const float thickness) {
	circle.setFillColor(fill);
	circle.setOutlineThickness(thickness);
	circle.setOutlineColor(outline);
	circle.setRadius(radius);
	circle.setPointCount(sides);
	circle.setOrigin(radius,radius);
}

cCollision::cCollision(const float radius) :
	collisionRadius(radius)
{
}

cScore::cScore(int s) :
	score(s) {
}

cLifeSpan::cLifeSpan(int total) :
	reamining(total),
	total(total) {
}

cInput::cInput() {
}

