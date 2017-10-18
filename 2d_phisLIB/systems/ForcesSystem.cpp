#include "ForcesSystem.h"



ForcesSystem::ForcesSystem(entityx::EventManager &ev)
{
	ev.subscribe<ApplyForceEvent>(*this);
}


ForcesSystem::~ForcesSystem()
{
}

void ForcesSystem::update(entityx::EntityManager & en, entityx::EventManager & ev, double dt)
{
	Mass::Handle massH;
	MOfInertia::Handle inerH;
	LinearVelocity::Handle LinVelH;
	AngularVelocity::Handle AngVelH;
	LinearForce::Handle LinForceH;
	AngularForce::Handle AngForceH;

	for (auto entity : en.entities_with_components(LinVelH, AngVelH, LinForceH, AngForceH, inerH, massH))
	{
		LinVelH->vel += LinForceH->force * massH->invMass;
		AngVelH->degree += AngForceH->force * 57.29577f * inerH->invI;// from radians to degrees;
		LinForceH->force = sf::Vector2f(0, 0);
		AngForceH->force = 0.0f;
	}
}

void ForcesSystem::receive(const ApplyForceEvent & ev)
{
	entityx::Entity en = ev.en;
	LinearForce::Handle LinForceH = en.component<LinearForce>();
	AngularForce::Handle AngForceH = en.component<AngularForce>();

	LinForceH->force += ev.force;
	AngForceH->force += CROSS(ev.point, ev.force);
}
