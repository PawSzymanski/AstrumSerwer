#pragma once
#include "LibsAndDeclarations.h"



class game_over_system : public entityx::System<game_over_system>
{
	bool doEnd;
	int licznik;
	entityx::Entity newEntity1;
	sf::Time time;
	sf::Clock clock;


	void release();
public:
	game_over_system();
	
	void update(entityx::EntityManager & en, entityx::EventManager & ev, double dt);

	~game_over_system();
};
