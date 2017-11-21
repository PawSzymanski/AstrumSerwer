#include "engine_system.h"
#include "ResourceManager.h"
/*
THIS FILE IS MODIFIED FOR MULTIPLAYER NEEDS
*/

engine_system::engine_system(entityx::EventManager &ev, Phisics_2D &phs) : phisics(phs)
{
	ev.subscribe<ApplyForceEvent>(*this);
}

void engine_system::update(entityx::EntityManager & en, entityx::EventManager & ev, double dt)
{
	auto &resource = ResourcesManager::getInstanceRef();
	isPlayer::Handle playerH;
	VertexArray::Handle verH;
	//Line::Handle lineH;
	Rotation::Handle rotH, rotEngH;
	Position::Handle posH, posPlayerH;
	
	AttachToPlayerPoint::Handle attachPointH;
	Transform::Handle transEngH, transPlayerH;
	PartInfo::Handle partInfH;
	//isLegs::Handle legsH;


	for (auto en1 : en.entities_with_components(posH, playerH, rotH))
	{
		if (playerH->id != resource.temporaryId)
			continue;

		for (auto en2 : en.entities_with_components(verH, transEngH, attachPointH, partInfH))
		{
			if (playerH->id != partInfH->playerId)
				continue;

			rotH = en1.component<Rotation>();
			posH = en1.component<Position>();

			verH = en2.component<VertexArray>();
			transEngH = en2.component<Transform>();
			rotEngH = en2.component<Rotation>();
			attachPointH = en2.component<AttachToPlayerPoint>();
			rotEngH = en2.component<Rotation>();
			transEngH->trans = transEngH->defaultTrans;

			//std::cout << posH->pos.x << "  " << posH->pos.y << std::endl;

			sf::Transform rotMatrix;
			rotMatrix.rotate(rotH->degree);

			attachPointH->point = rotMatrix * attachPointH->point;

			transEngH->trans.translate(posH->pos + attachPointH->point);
			transEngH->trans.rotate(rotH->degree + rotEngH->degree);
			//update

			//parts functions///////////////////////////////////////////////////////////////////////////
			
			enginePart(en1, en2, ev, attachPointH, rotMatrix, partInfH);
			gunPart(en, en1, en2, ev, attachPointH, rotMatrix, rotEngH->degree);

			//end;/////////////////////////////////////////////////////////////////////////////////////////
			attachPointH->point = rotMatrix.getInverse() * attachPointH->point;	
		}
	}
}

void engine_system::enginePart(entityx::Entity enPlayer, entityx::Entity enPart, 
	entityx::EventManager & ev, AttachToPlayerPoint::Handle attachPointH, sf::Transform rotMatrix, PartInfo::Handle partInfH)
{
	if (!enPart.has_component<isEngine>())
	{
		return;
	}
	auto & resource = ResourcesManager::getInstanceRef();

	KeyAssigned::Handle keyH;
	ForcePoint::Handle pointH;
	AdditionalAnim::Handle animH;
	
	pointH = enPart.component<ForcePoint>();
	keyH = enPart.component<KeyAssigned>();

	pointH->force = rotMatrix * pointH->force;

	if (partInfH->isActive)
	{	
		ev.emit<ApplyForceEvent>(attachPointH->point, pointH->force, enPlayer);
	}
	pointH->force = rotMatrix.getInverse() * pointH->force;
}

void engine_system::gunPart(entityx::EntityManager & en, entityx::Entity enPlayer, entityx::Entity enPart, entityx::EventManager & ev,
	AttachToPlayerPoint::Handle attachPointH, sf::Transform rotMatrix, float degreeOfGun)
{
	if (!enPart.has_component<isGun>())
	{
		return;
	}
	KeyAssigned::Handle keyH;
	keyH = enPart.component<KeyAssigned>();

	Position::Handle posH;
	posH = enPlayer.component<Position>();
	
	isBullet::Handle bulletH;

	sf::Transform rotTransformGun;
	rotTransformGun.rotate(degreeOfGun);

	if (bulletTime.asMilliseconds() > 80 && sf::Keyboard::isKeyPressed(keyH->key))
	{
		auto bulletEn = en.create();
		bulletTime = sf::Time::Zero;
		phisics.createPolygon(bulletEn, posH->pos + attachPointH->point,
			rotTransformGun * rotMatrix * sf::Vector2f(0, -14), 0, 0.01, "bullet");
		bulletEn.assign<isBullet>(false);
	}
	for (auto bullets : en.entities_with_components(bulletH))
	{
		bulletH->livingTime += bulletClock.getElapsedTime();
		if (bulletH->livingTime.asSeconds() > 4)
		{
			bullets.destroy();
		}
		else if (bulletH->livingTime.asSeconds() > 1.5 && bulletH->isEnemy)
		{
			bullets.destroy();
		}
	}
	bulletTime += bulletClock.restart();
}

void engine_system::receive(const ApplyForceEvent & ev)
{
	LinearVelocity::Handle velH;
	entityx::Entity en = ev.en;
	velH = en.component<LinearVelocity>();
}

engine_system::~engine_system()
{
}
