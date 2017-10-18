#include "GameplayStage.h"


GamePlayStage::GamePlayStage() :time(sf::Time::Zero), isWaitingForPlayers(true), isInit(false)
{
}
bool GamePlayStage::isFull()
{
	std::cout << std::endl;
	for(auto&p :player)
	std::cout << p.uniqueId  << std::endl;
	if (player.size() > 1)
	{
		return true;
	}
	return false;
}

bool GamePlayStage::waitingForPlayers(std::string & str)
{
	if (isFull())
	{
		init();
		return false;
	}
	else
	{
		std::cout << "waiting" << std::endl;
		return true;
	}
}
bool GamePlayStage::init()
{
	if (isInit)
		return true;
	
	ex_ptr = std::make_shared<entityx::EntityX>();
	phisics_ptr = std::make_shared<Phisics_2D>((*ex_ptr), ResourcesManager::getInstanceRef().vertCont, sf::Vector2f(0, 3));
	(*phisics_ptr).init();
	int i = 0;
	for (auto &p : player)
	{
		i++;
		entityx::Entity en;
		(*phisics_ptr).createPolygon(en, sf::Vector2f(i, 0), sf::Vector2f(0, 0), 0, 1, p.bodyName);
	}
	
	auto &resource = ResourcesManager::getInstanceRef();
	resource.camera.reset(sf::FloatRect(0, 0, 17.5, 10));

	ResourcesManager::getInstanceRef().areAllPlatfIncluded = false;

	//(*ex_ptr).systems.add<player_input_system>(*phisics_ptr);
	//(*ex_ptr).systems.update<player_input_system>(0.1);

	(*ex_ptr).systems.add<engine_system>((*ex_ptr).events, *phisics_ptr);
	(*ex_ptr).systems.add<DestructionSystem>(*ex_ptr);
	(*ex_ptr).systems.add<CraneSystem>(*ex_ptr, *phisics_ptr);
	(*ex_ptr).systems.add<platform_manager>((*ex_ptr).events);
	(*ex_ptr).systems.add<CargoSystem>();
	(*ex_ptr).systems.add<game_over_system>();
	(*ex_ptr).systems.add<enemy_system>((*ex_ptr), *phisics_ptr);
	(*ex_ptr).systems.add<sliding_doors_system>();
	
	isInit = true;
	return true;
}
bool GamePlayStage::update(std::string &buffer)
{

	time = sf::Time::Zero;
	time += clock.restart();
	
	while (time.asSeconds() >= dt && isWaitingForPlayers == false)
	{
		//do all phisics
		(*ex_ptr).systems.update<engine_system>(dt);
		(*ex_ptr).systems.update<DestructionSystem>(dt);
		(*ex_ptr).systems.update<CraneSystem>(dt);
		(*ex_ptr).systems.update<platform_manager>(dt);
		(*ex_ptr).systems.update<CargoSystem>(dt);
		(*ex_ptr).systems.update<game_over_system>(dt);
		(*ex_ptr).systems.update<enemy_system>(dt);
		(*ex_ptr).systems.update<sliding_doors_system>(dt);
		

		//
		time -= sf::seconds(dt);
	}

	return true;
}
bool GamePlayStage::release()
{

}
GamePlayStage::~GamePlayStage()
{
}
