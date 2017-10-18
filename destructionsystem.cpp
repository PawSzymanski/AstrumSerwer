/*
 *  destructionsystem.cpp
 *
 *  Created: 2017-07-29
 *   Author: Patryk Wojtanowski
 */

#include "destructionsystem.h"
#include "ResourceManager.h"

DestructionSystem::DestructionSystem(entityx::EntityX &ex)
{
    ex.events.subscribe<CollisionEvent>(*this);
}

void DestructionSystem::update(entityx::EntityManager &en, entityx::EventManager &ev, double dt)
{
}

void DestructionSystem::receive(const CollisionEvent &ev)
{
    auto en1 = ev.en1;
    auto en2 = ev.en2;
	isBullet::Handle bulletH;

	if (en1.has_component<isBullet>() || en2.has_component<isBullet>())
	{
		if ((en1.has_component<isPlayer>() || en2.has_component<isPlayer>()) && ev.relVel > 100)
		{
			bool en1_is_bullet = en1.has_component<isBullet>(),
				en2_is_bullet = en2.has_component<isBullet>();

			bullet = (en1_is_bullet) ? en1 : en2;
			bulletH = bullet.component<isBullet>();

			if ((!en1_is_bullet && !en2_is_bullet))
				return;

			bool en1_is_player = en1.has_component<isPlayer>(),
				en2_is_player = en2.has_component<isPlayer>();

			player = (en1_is_player) ? en1 : en2;

			if(bulletH->isEnemy)
			boomAnimation(player);
		}
		return;
	}
    bool en1_is_player = en1.has_component<isPlayer>(),
         en2_is_player = en2.has_component<isPlayer>();

    if((!en1_is_player && !en2_is_player) || ev.relVel < 100.0f)
        return;

    player = (en1_is_player)? en1 : en2;

	boomAnimation(player);

    //std::cout<<"destruction system receive: "<<ev.relVel<<std::endl;
}

//activate animation, and ends game
void DestructionSystem::boomAnimation(entityx::Entity player) {
	if (player.has_component<AdditionalAnim>())
		return;
	
	Transform::Handle transH;

	transH = player.component<Transform>();

	auto & resource = ResourcesManager::getInstanceRef();


	ResourcesManager::getInstanceRef().isGameOver = true;
}