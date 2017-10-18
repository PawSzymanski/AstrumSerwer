#pragma once

#include <SFML/Graphics.hpp>
#include <entityx/entityx.h>

struct ApplyForceEvent
{
	ApplyForceEvent(sf::Vector2f point, sf::Vector2f force, entityx::Entity en)
		:point(point), force(force), en(en) {}
	sf::Vector2f point, force;
	entityx::Entity en;
};

struct CollisionEvent
{
    CollisionEvent(entityx::Entity en1, entityx::Entity en2, float relVel, sf::Vector2f contact)
        :en1(en1), en2(en2), relVel(relVel), contact(contact){}
    entityx::Entity en1, en2;
    float relVel;
    sf::Vector2f contact;
};
