#pragma once
#include "LibsAndDeclarations.h"

class platform_manager : public entityx::System<platform_manager>, public entityx::Receiver<platform_manager>
{
	bool isPlatColliding;
	sf::Clock clock;
	sf::Time time;
	std::unique_ptr<CollisionEvent> evTemp_ptr;
public:
	platform_manager(entityx::EventManager &ev);
	~platform_manager();

	void update(entityx::EntityManager & en, entityx::EventManager & ev, double dt);

	void receive(const CollisionEvent & ev);
};

