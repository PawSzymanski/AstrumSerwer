#include "game_over_system.h"
#include "ResourceManager.h"

game_over_system::game_over_system() :doEnd(false), licznik(0)
{
	clock.restart();
}

void game_over_system::release()
{
	ResourcesManager::getInstanceRef().areAllPlatfIncluded = false;
	ResourcesManager::getInstanceRef().areAllCargoSpaceIncluded = false;
	ResourcesManager::getInstanceRef().isGameOver = false;


	ResourcesManager::getInstanceRef().isPauseTime = false;

	newEntity1.destroy();
}

void game_over_system::update(entityx::EntityManager & en, entityx::EventManager & ev, double dt)
{
	auto &resource = ResourcesManager::getInstanceRef();
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		resource.isPauseTime = true;
	}
	//std::cout << resource.areAllPlatfIncluded << resource.areAllCargoSpaceIncluded << std::endl;

	if ( ( resource.areAllPlatfIncluded == true && resource.areAllCargoSpaceIncluded == true ) ||
					resource.isGameOver == true || 
						resource.isPauseTime == true)
	{
		std::cout << "DRAW MENU" << std::endl;

		if ((resource.areAllPlatfIncluded == true &&
				resource.areAllCargoSpaceIncluded == true) || 
					!resource.isPauseTime)
		{
		
		}
		resource.isGameOver = true;

		ResourcesManager::getInstanceRef().isGameOver = true;
		
		if (!newEntity1.valid())
		{
			clock.restart();
			newEntity1 = en.create();

			newEntity1.assign<VertexArray>(ResourcesManager::getInstanceRef().vertCont.getPoly("faded_screen"),
			ResourcesManager::getInstanceRef().vertCont.getNormals("faded_screen"));
			newEntity1.assign<PolyName>("faded_screen3"); // just for render to let know what texture 
			newEntity1.assign<Transform>(sf::Vector2f(0, 0), 0);
			VertexArray::Handle txtToRender = newEntity1.component<VertexArray>();
		 }
		
		time += clock.restart();
		int buttPos = -1;
		//game over MENU case

	}
	else
	{ }
	
	AdditionalAnim::Handle additH;
	for (auto e : en.entities_with_components(additH))
	{
		additH = e.component<AdditionalAnim>();
		if (additH->wholeTime.asSeconds() > 4 && additH->nameOfAnim == "explosion")
		{
			e.remove<AdditionalAnim>();
		}
	}
}


game_over_system::~game_over_system()
{
}
