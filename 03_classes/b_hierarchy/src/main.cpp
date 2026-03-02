/**
 * Exercise: Entity Hierarchy and Polymorphism
 *
 * Implement the methods declared in entity.hpp inside entity.cpp.
 * Use dynamic_cast to identify entity types and implement type-specific behaviors.
 *
 * Expected behaviors:
 *
 * Player :
 *   - update(): "Player <name> explores the world"
 *   - interact(Enemy):  reduce enemy.health by 20, "Player <name> strikes <enemy> with a sword! (-20 HP)"
 *   - interact(NPC):    gain 5 health, "Player <name> receives a healing potion from <npc> (+5 HP)"
 *   - interact(other):  "Player <name> waves at <other>"
 *
 * Enemy :
 *   - update(): "Enemy <name> is roaming"
 *   - interact(Player): reduce player.health by 10, "Enemy <name> attacks <player>! (-10 HP)"
 *   - interact(Enemy):  gain 3 health, "Enemy <name> forms an alliance with <enemy> (+3 HP)!"
 *   - interact(other):  "Enemy <name> growls at <other>"
 *
 * NPC :
 *   - update(): "NPC <name> is waiting"
 *   - interact(Player): "NPC <name> says: Greetings, <player>! I have a quest for you."
 *   - interact(other):  "NPC <name> ignores <other>"
 */

// main.cpp
#include <vector>
#include <iostream>

#include "entity.hpp"

int main()
{
    
    // TO DO: Make the following code work by implementing the required behaviors in entity.hpp and entity.cpp
/*
    Player player("Hero");
    Enemy enemy1("Goblin");
    Enemy enemy2("ORC");
    NPC npc1("Villager");

    std::vector<Entity*> entities;
    entities.push_back(&player);
    entities.push_back(&enemy1);
    entities.push_back(&enemy2);
    entities.push_back(&npc1);

    int nbr_round = 2;
    for(int k=0; k<nbr_round; ++k) {

        std::cout << "\nRound " << (k+1) << "\n\n";

        std::cout << "--- UPDATE LOOP ---\n";
        for (Entity* e : entities) {
            e->update(); // polymorphisme
        }

        std::cout << "\n--- INTERACTIONS ---\n";
        for(Entity* e1 : entities) {
            for(Entity* e2 : entities) {
                if(e1 != e2) {
                    e1->interact(*e2); // polymorphisme
                }
            }
        }
        
        std::cout << "\n--- STATE ---\n";
        for(Entity* e1 : entities)
            std::cout << e1->name << " health: " << e1->health << "\n";
    }
*/
return 0;
}
