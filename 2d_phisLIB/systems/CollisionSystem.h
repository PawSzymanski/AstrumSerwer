#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "../isCollidingFuns.h"
#include "../components.h"
#include "../Events.h"
#include "../AutisticCmath.h"
#include "../Events.h"

class CollisionSystem : public entityx::System<CollisionSystem>
{

	sf::Vector2f &gravity;
public:
	void(*dispatch[2][2])(Manifold& m);

	CollisionSystem(sf::Vector2f &);

	void PositionalCorrection(Manifold & m);

	void ResolveCollision(Manifold & m, entityx::EventManager & ev, float dt);

	void update(entityx::EntityManager & en, entityx::EventManager & ev, double dt);

	~CollisionSystem();
};
