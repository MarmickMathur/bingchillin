#pragma once
#include <string>
#include "Components.h"

class Entity {

	size_t m_id;
	std::string m_tag;
	bool active;

	Entity(size_t id , std::string tag );

public:
	std::shared_ptr<cTransform> cTranfromPtr = nullptr;
	std::shared_ptr<cShape>     cShapePtr    = nullptr;
	std::shared_ptr<cScore>     cScorePtr    = nullptr;
	std::shared_ptr<cLifeSpan>  cLifeSpanPtr = nullptr;
	std::shared_ptr<cInput>     cInputPtr    = nullptr;
	std::shared_ptr<cCollision> cCollisionPtr = nullptr;

	void destroy();
	bool isActive() const;
	const size_t id() const;
	const std::string& tag() const;

	friend class EntityManager;
};