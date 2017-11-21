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
#include "Conversion.h"
/*
GAMEPLAY STAGE IS EQUIVALENT TO ONE SESSION 
*/
class GamePlayStage : public SerwerStage
{
	
	sf::Clock clock;
	sf::Time time;
	std::string playersInfoStr;
	std::shared_ptr<entityx::EntityX> ex_ptr;
	std::shared_ptr<Phisics_2D> phisics_ptr;
	bool isInit;
	bool LoadPart(ResourcesManager::UniquePlayer player);
public:
	const float dt = 0.01;
	GamePlayStage();
	bool init(std::string &);
	bool isWaitingForPlayers;

	std::vector<ResourcesManager::UniquePlayer> player;
	bool waitingForPlayers(std::string &str);
	bool decodeDataFromClient(std::string & str);
	bool collectDataToSend(std::string & str);
	bool updateVelocity(unsigned int playersId, std::string & bufferStr);
	bool updatePartsAction(unsigned int playersId, std::string & bufferStr);

	void addPlayersInfoToBuff(unsigned int playersId, std::string & buffer);
	
	bool isFull();
	
	bool update(std::string &);
	bool release();
	~GamePlayStage();
};

