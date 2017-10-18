#pragma once

#include <entityx/entityx.h>
#include "../components.h"

class MovementSystem : public entityx::System<MovementSystem>
{
public:
	MovementSystem();
	~MovementSystem();

	void update(entityx::EntityManager & en, entityx::EventManager &ev, double dt);
};

