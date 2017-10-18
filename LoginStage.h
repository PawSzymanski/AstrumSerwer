#pragma once
#include "LibsAndDeclarations.h"
#include "SerwerStage.h"
#include "GameplayStage.h"
#include "ResourceManager.h"
class LoginStage : public SerwerStage
{
	std::string loginStr, passStr;
public:
	LoginStage();
	bool checkIfNumerExistInBase(unsigned int i);
	unsigned int giveUniqueNum();
	bool receiveDataFromClient(std::string &bufferStr);
	bool doExistUserInBase();
	bool update(std::string &);

	bool waitingForPlayers(std::string &str);

	~LoginStage();
};

