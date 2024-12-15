#pragma once
#include <vector>
#include <map>
#include "Entity.h"
#include <memory>
#include <string>

typedef std::vector<std::shared_ptr<Entity> > entityVec;
typedef std::map<std::string, entityVec> entityMap;

class EntityManager {
	entityVec m_entities;
	entityVec m_addLater;
	entityMap m_entityMap;
	size_t m_totalEntities = 0;

	void RemoveDeadEntities(entityVec& entityVec);
public:
	EntityManager();
	void update();
	std::shared_ptr<Entity> addEntity(const std::string &tag);
	entityVec getEntities();
	entityVec getEntities(const std::string &tag);
};