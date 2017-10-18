/*
 *  cargosystem.h
 *
 *  Created: 2017-07-06
 *   Author: Patryk Wojtanowski
 */

#ifndef CARGOSYSTEM_H
#define CARGOSYSTEM_H

#include "LibsAndDeclarations.h"

class CargoSystem : public entityx::System<CargoSystem>
{
public:
    CargoSystem();

    void update(entityx::EntityManager & en, entityx::EventManager &ev, double dt);
};

#endif // CARGOSYSTEM_H
