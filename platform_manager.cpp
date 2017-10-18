#include "platform_manager.h"
#include "ResourceManager.h"


platform_manager::platform_manager(entityx::EventManager &ev) : isPlatColliding(false) 
{
	ev.subscribe<CollisionEvent>(*this);
	evTemp_ptr = std::make_unique<CollisionEvent>(entityx::Entity(), entityx::Entity(), 0.0, sf::Vector2f(0,0));
}


platform_manager::~platform_manager()
{
	ResourcesManager::getInstanceRef().areAllPlatfIncluded = false;
}

void platform_manager::update(entityx::EntityManager & en, entityx::EventManager & ev, double dt)
{
	int n = 0;
	for (auto ent : en.entities_with_components<isPlatform>())
	{
		++n;
	}
	if (n == 0)
	{
		ResourcesManager::getInstanceRef().areAllPlatfIncluded = true;
	}
	else if (isPlatColliding) 
	{
		isPlatform::Handle platH = (*evTemp_ptr).en1.has_component<isPlatform>() ? (*evTemp_ptr).en1.component<isPlatform>() : (*evTemp_ptr).en2.component<isPlatform>();
		if (clock.getElapsedTime().asMilliseconds() < 50)
		{
			time += clock.getElapsedTime();
            std::cout << clock.getElapsedTime().asMilliseconds() << "    " << time.asMilliseconds() << std::endl;

			if (time.asSeconds() > 1)
			{
				std::cout << "PASSED " << std::endl;
				platH->isPassed = true;
			}
		}
		else
		{
			time = sf::Time::Zero;
		}
		int i = 0, amountOfPlatforms = 0;

		for (auto ent : en.entities_with_components<isPlatform>())
		{
			++amountOfPlatforms;
		}
		//are all platform passed
		for (auto ent : en.entities_with_components<isPlatform>())
		{
			++i;
			platH = ent.component<isPlatform>();
			if (!platH->isPassed)
			{
				break;
			}
			else if(i >= amountOfPlatforms)
			{
				ResourcesManager::getInstanceRef().areAllPlatfIncluded = true;
			}
		}
		isPlatColliding = false;
		clock.restart();
	}
	else 
	{
	}
}

void platform_manager::receive(const CollisionEvent & ev)
{
	//LinearVelocity::Handle velH;

	entityx::Entity en = ev.en1;
	
	if (ev.en1.has_component<isPlatform>() && ev.en2.has_component<isPlayer>()
		|| ev.en1.has_component<isPlayer>() && ev.en2.has_component<isPlatform>())
	{
		isPlatColliding = true;
		(*evTemp_ptr) = ev;
	}

}
