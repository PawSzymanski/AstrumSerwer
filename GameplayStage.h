#pragma once
#include "LibsAndDeclarations.h"
#include "2d_phisLIB/phisics_2d.h"
#include "ResourceManager.h"

#include "cargosystem.h"
#include "enemy_system.h"
#include "cranesystem.h"
#include "destructionsystem.h"
#include "game_over_system.h"
#include "engine_system.h"
#include "sliding_doors_system.h"
#include "platform_manager.h"

class GamePlayStage : public SerwerStage
{
	const float dt = 0.01;
	sf::Clock clock;
	sf::Time time;
	std::shared_ptr<entityx::EntityX> ex_ptr;
	std::shared_ptr<Phisics_2D> phisics_ptr;
	bool isInit;
public:
	GamePlayStage();
	bool isWaitingForPlayers;
	std::vector<ResourcesManager::UniquePlayer> player;
	bool waitingForPlayers(std::string &str);
	bool init();
	bool isFull();
	
	bool update(std::string &);
	bool release();
	~GamePlayStage();
};

