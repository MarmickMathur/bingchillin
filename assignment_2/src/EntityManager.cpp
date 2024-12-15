#include "EntityManager.h"
#include <iostream>

EntityManager::EntityManager() {

};

void EntityManager::RemoveDeadEntities(entityVec& entityVec) {
	auto ne = std::remove_if(entityVec.begin(), entityVec.end(),
		[](auto i) {
			return !(i->isActive());
		});
	entityVec.erase(ne, entityVec.end());
}


void EntityManager::update() {
	//std::cout << "updating" << "\n";
	for (auto& i : m_addLater) {
		m_entities.push_back(i);
		m_entityMap[i->tag()].push_back(i);
	}
	m_addLater.clear();

	RemoveDeadEntities(m_entities);
	for (auto &i : m_entityMap) {
		//std::cout << "removed " << i.first << " ";
		RemoveDeadEntities(i.second);
		std::cout << i.second.size() << "\n";
	}
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag) {
	//std::cout << "adding entity ";
	std::shared_ptr<Entity> e = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	m_addLater.push_back(e);
	//std::cout << m_addLater.size() << "\n";
	return e;
}

entityVec EntityManager::getEntities() {
	return m_entities;
}


entityVec EntityManager::getEntities(const std::string& tag) {
	return m_entityMap[tag];
};