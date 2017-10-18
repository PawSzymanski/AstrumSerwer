#pragma once

#include "LibsAndDeclarations.h"
#include "ResourceManager.h"
#include "GameplayStage.h"

class LoadingDataStage : public SerwerStage
{
public:
	std::vector<GamePlayStage>& gamePlaySt;

	LoadingDataStage(std::vector<GamePlayStage> &);
	bool receiveDataFromClient(std::string & bufferStr);
	int assignToSession(ResourcesManager::UniquePlayer & unPlayer);
	void loadPlayer(std::string & bufferStr, ResourcesManager::UniquePlayer &player);

	bool waitingForPlayers(std::string &str);

	bool update(std::string &);
	~LoadingDataStage();
};

