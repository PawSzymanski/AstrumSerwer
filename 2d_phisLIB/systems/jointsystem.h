#pragma once

#include <entityx/entityx.h>
#include "../components.h"
#include "../AutisticCmath.h"
#include "../Events.h"

class JointSystem : public entityx::System<JointSystem>
{
    sf::Vector2f resolve(Joint::Handle joint, float& sep, sf::Vector2f &normal);
    void correct(Joint::Handle joint, float sep, sf::Vector2f normal);
public:
    JointSystem();
    void update(entityx::EntityManager & en, entityx::EventManager & ev, double dt);
};

