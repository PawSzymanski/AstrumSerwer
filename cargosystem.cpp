/*
 *  cargosystem.cpp
 *
 *  Created: 2017-07-06
 *   Author: Patryk Wojtanowski
 */

#include "cargosystem.h"
#include "ResourceManager.h"

CargoSystem::CargoSystem()
{

}

void CargoSystem::update(entityx::EntityManager &en, entityx::EventManager &ev, double dt)
{
    CargoSpace::Handle cargo_space;
    Cargo::Handle cargoH;
    Position::Handle pos;

    for(auto entity: en.entities_with_components(cargo_space, pos))
    {
        entityx::Entity cargo;
        for(auto entity1 : en.entities_with_components(cargoH))
            if(cargoH->id == cargo_space->id) 
			{ 
				cargo = entity1; 
				break; 
			}

        Position::Handle cargo_posH = cargo.component<Position>();
		std::cout << "cargo X: " << cargo_posH->pos.x << "Y: " << cargo_posH->pos.y << std::endl;
        sf::Vector2f cargo_pos = cargo_posH->pos;
        sf::Vector2f space_pos = pos->pos;
        sf::Vector2f dim = cargo_space->dim + pos->pos;
		std::cout << std::endl;
		//std::cout << cargo_pos.x << " " << cargo_pos.y <<" :: " << dim.x <<" "<< dim.y <<" :: "<< space_pos.x << " " << space_pos.y <<std::endl;

        if(cargo_pos.x > space_pos.x && cargo_pos.x < dim.x &&
                cargo_pos.y > space_pos.y && cargo_pos.y < dim.y)
        {
            if(!cargo_space->checked)
                cargo_space->time += sf::seconds(dt);
            if(cargo_space->time.asSeconds() > 3.0f)
                cargo_space->checked = true;

            std::cout<<"IN CARGO SPACE for: "<<cargo_space->time.asSeconds()<<std::endl;
        }
        else
            cargo_space->time = sf::Time::Zero;
    }





	ResourcesManager::getInstanceRef().areAllCargoSpaceIncluded = true;
	for (auto en1 : en.entities_with_components(cargo_space))
	{
		if (!cargo_space->checked)
		{
			ResourcesManager::getInstanceRef().areAllCargoSpaceIncluded = false;
			break;
		}
		else
		{
			ResourcesManager::getInstanceRef().areAllCargoSpaceIncluded = true;
		}
	}
}
