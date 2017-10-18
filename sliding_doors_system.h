#pragma once
#include "LibsAndDeclarations.h"

class sliding_doors_system : public entityx::System<sliding_doors_system>
{
	float dist;
public:
	sliding_doors_system();

	void update(entityx::EntityManager & en, entityx::EventManager & ev, double dt);

	void openDoors(entityx::Entity);

	~sliding_doors_system();
};

