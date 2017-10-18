#ifndef PHISICS_2D_H
#define PHISICS_2D_H


#include <entityx/entityx.h>
#include "systems/CollisionSystem.h"
#include "systems/ForcesSystem.h"
#include "systems/MovementSystem.h"
#include "systems/jointsystem.h"
#include "Container.h"
#include "components.h"

class Phisics_2D
{
    entityx::EntityX &ex;
    Container & cont;
    sf::Vector2f &gravity;

public:
    Phisics_2D(entityx::EntityX & ex, Container &cont, sf::Vector2f &gravity);
    void init();
    void update(float dt);

    void createCircle(entityx::Entity en, sf::Vector2f pos, sf::Vector2f vel, float mass, sf::Color col, float r);
    void createPolygon(entityx::Entity en, sf::Vector2f pos, sf::Vector2f vel, float rotation, float mass, const std::string &polyname);
    entityx::Entity createJoint(entityx::Entity en1, entityx::Entity en2, sf::Vector2f pos1, sf::Vector2f pos2, float lenght);

};

#endif // PHISICS_2D_H
