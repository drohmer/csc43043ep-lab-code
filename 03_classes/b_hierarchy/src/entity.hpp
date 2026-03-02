// entity.hpp
#pragma once
#include <string>

class Entity {
public:
    Entity(std::string const& n);
    virtual ~Entity() = default;

    // Own behavior of the entity
    virtual void update() = 0;

    // Default interaction: does nothing
    virtual void interact(Entity& other);

    std::string name; // Each entity has a name
    int health = 100; // Each entity has health points
};
