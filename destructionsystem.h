/*
 *  destructionsystem.h
 *
 *  Created: 2017-07-29
 *   Author: Patryk Wojtanowski
 */

#ifndef DESTRUCTIONSYSTEM_H
#define DESTRUCTIONSYSTEM_H

#include "LibsAndDeclarations.h"
#include "2d_phisLIB/Events.h"
#include "2d_phisLIB/components.h"

class DestructionSystem : public entityx::System<DestructionSystem>, public entityx::Receiver<DestructionSystem>
{
    AdditionalAnim::Handle boom_anim;
    entityx::Entity player, bullet;

public:
    DestructionSystem(entityx::EntityX &ex);

    void update(entityx::EntityManager & en, entityx::EventManager & ev, double dt);

    void receive(const CollisionEvent & ev);

	void boomAnimation(entityx::Entity player);
};

#endif // DESTRUCTIONSYSTEM_H
