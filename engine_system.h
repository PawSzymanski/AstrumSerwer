#pragma once
#include "LibsAndDeclarations.h"

class engine_system : public entityx::System<engine_system>, public entityx::Receiver<engine_system>
{
	bool boolHelp;
	sf::Transform matrix;
	Phisics_2D & phisics;
	sf::Clock bulletClock;
	sf::Time bulletTime;

	void enginePart(entityx::Entity enPlayer, entityx::Entity enPart, entityx::EventManager & ev, AttachToPlayerPoint::Handle attachPointH, sf::Transform rotMatrix);
	void gunPart(entityx::EntityManager & en, entityx::Entity enPlayer, entityx::Entity enPart, entityx::EventManager & ev,
				AttachToPlayerPoint::Handle attachPointH, sf::Transform rotMatrix, float);
public:
	engine_system(entityx::EventManager &ev, Phisics_2D &phs);

	void update(entityx::EntityManager & en, entityx::EventManager &ev, double dt);

	void receive(const ApplyForceEvent & ev);

	~engine_system();
};

