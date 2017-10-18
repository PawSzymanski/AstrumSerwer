#pragma once
//#define CROSS(a,b) (a.x * b.y - a.y * b.x)


#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include "../Events.h"
#include "../components.h"
#include "../AutisticCmath.h"

#define CROSS(a,b) (a.x * b.y - a.y * b.x)

class ForcesSystem : public entityx::System<ForcesSystem>, public entityx::Receiver<ForcesSystem>
{
public:
	ForcesSystem(entityx::EventManager &ev);
	~ForcesSystem();

	void update(entityx::EntityManager &en, entityx::EventManager& ev, double dt);

	void receive(const ApplyForceEvent & ev);
};

