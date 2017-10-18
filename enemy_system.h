#pragma once
#include "LibsAndDeclarations.h"


class enemy_system : public entityx::System<enemy_system>, public entityx::Receiver<enemy_system>
{
	bool dir;
	sf::Vector2f detectionSpace ;
	Phisics_2D & phisics;
	sf::Clock bulletClock;
	sf::Time bulletTime;
public:
	enemy_system(entityx::EntityX &ex, Phisics_2D &);
	void update(entityx::EntityManager & en, entityx::EventManager & ev, double dt);
	void cameraDesrtoy(const CollisionEvent & ev);
	void receive(const CollisionEvent & ev);
	~enemy_system();
};


