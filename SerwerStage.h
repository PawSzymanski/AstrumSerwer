#pragma once
#include "LibsAndDeclarations.h"
class SerwerStage
{
public:
	SerwerStage();
	virtual bool update(std::string &) = 0;
	virtual bool waitingForPlayers(std::string &str) = 0;
	~SerwerStage();
};

