#include "sliding_doors_system.h"



sliding_doors_system::sliding_doors_system() : dist(0)
{
}

void sliding_doors_system::update(entityx::EntityManager & en, entityx::EventManager & ev, double dt)
{
	isSlidingDoors::Handle doorsH;

	//checking conditions to open doors;
	for (auto e : en.entities_with_components<>(doorsH))
	{
		if (!doorsH->isOpen)
		{
			std::cout << "not all doors open" << std::endl;
			for (int i = 0; i < doorsH->opener.size(); ++i)
			{
				//you can add more things that can be passed (one "if" one thing)
				if (doorsH->opener[i] == "platform")
				{
					isPlatform::Handle platH;
					for (auto plat : en.entities_with_components<>(platH))
					{
						if (doorsH->id[i] == platH->id)
						{
							if (!platH->isPassed)
							{
								doorsH->isOpen = false;
								break;
							}
							else
							{
								doorsH->isOpen = true;
							}
						}
					}
				}
				else if (doorsH->opener[i] == "cargo_space")
				{
					CargoSpace::Handle cargoH;
					for (auto cargo : en.entities_with_components<>(cargoH))
					{
						if (doorsH->id[i] == cargoH->id)
						{
							if (!cargoH->checked)
							{
								doorsH->isOpen = false;
								break;
							}
							else
							{
								doorsH->isOpen = true;
							}
						}
					}
				}
			}
		}
		//if good open
		else
		{
			openDoors(e);
		}
	}
}

void sliding_doors_system::openDoors(entityx::Entity ent)
{
	Position::Handle posH = ent.component<Position>();
	Rotation::Handle rotH = ent.component<Rotation>();
	isSlidingDoors::Handle doorsH= ent.component<isSlidingDoors>();
	if (doorsH->distance < 3)
	{
		sf::Transform trans;
		trans.rotate(rotH->degree);

		posH->pos += trans * sf::Vector2f(0.1, 0);
		doorsH->distance += 0.1;
	}
}


sliding_doors_system::~sliding_doors_system()
{
}
