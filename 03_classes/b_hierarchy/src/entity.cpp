// entity.cpp
#include "entity.hpp"
#include <iostream>

// Entity
Entity::Entity(std::string const& n) : name(n) {}

void Entity::interact(Entity& other) {
    std::cout << name << " doesn't interact with " << other.name << "\n";
}
