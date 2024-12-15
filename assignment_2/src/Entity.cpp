#include "Entity.h"


Entity::Entity(size_t id, std::string tag):
	m_id(id),
	m_tag(tag)
{
	active = 1;
}

bool Entity::isActive() const {
	return active;
}

void Entity::destroy() {
	active = 0;
}

const size_t Entity::id() const  {
	return m_id;
}

const std::string& Entity::tag() const {
	return m_tag;
}