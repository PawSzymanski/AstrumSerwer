/*
 *  partbutton.h
 *
 *  Created: 2017-08-06
 *   Author: Patryk Wojtanowski
 *
 */

#ifndef CRANESYSTEM_H
#define CRANESYSTEM_H

#include <entityx/entityx.h>
#include "2d_phisLIB/Events.h"
#include "2d_phisLIB/components.h"
#include "2d_phisLIB/phisics_2d.h"

#define HOOK_LENGHT 2.0f

class CraneSystem : public entityx::System<CraneSystem>, public entityx::Receiver<CraneSystem>
{
    Phisics_2D & phisics;

    entityx::Entity player;
    Position::Handle play_pos;
    Rotation::Handle play_rot;

    CollisionEvent * c_ev;

public:
    CraneSystem(entityx::EntityX & ex, Phisics_2D &phisiscs);

    void update(entityx::EntityManager & en, entityx::EventManager &ev, double dt);

    void receive(const CollisionEvent & ev);
};

#endif // CRANESYSTEM_H
