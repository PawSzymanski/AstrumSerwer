#include "LoadingDataStage.h"
#include "ResourceManager.h"


LoadingDataStage::LoadingDataStage(std::vector<GamePlayStage> &gamePlay) : gamePlaySt(gamePlay)
{
}

bool LoadingDataStage::receiveDataFromClient(std::string &bufferStr)
{

}
//returns and assigns session that player was assigned to
int LoadingDataStage::assignToSession(ResourcesManager::UniquePlayer &unPlayer)
{
	int i = 1;
	for (auto &session : gamePlaySt)
	{
		for (auto &players : session.player)
		{
			if (players.uniqueId == unPlayer.uniqueId)
				return i;
		}

		if (session.isFull())
		{
			
			std::cout << std::endl;
			std::cout << "isFull:" << i << std::endl;
			++i;
			continue;
		}
		else
		{
			std::cout << std::endl;
			std::cout << "PUSHED BACK PLAYER!" << std::endl;
			session.player.push_back(unPlayer);
			
			return i;
		}
	}
		GamePlayStage g;
		gamePlaySt.push_back(g);
		std::cout << "PUSHED BACK PLAYER!" << std::endl;
		gamePlaySt[gamePlaySt.size()-1].player.push_back(unPlayer);
		return gamePlaySt.size();
}
void LoadingDataStage::loadPlayer(std::string &bufferStr, ResourcesManager::UniquePlayer &player)
{
	auto &resource = ResourcesManager::getInstanceRef();

	std::string data = "NULL";
	std::cout << "acual buffer:" << bufferStr << std::endl;
	if (player.parts.namePart.size() > 1)
		return;

	while (data != "")
	{	
		data = resource.decodeOneLineDel(bufferStr);

		std::cout << "data:" << data << std::endl;

		if (data == "@body_info")
		{
			std::cout << "acual buffer:" << bufferStr << std::endl;
			player.bodyName = resource.decodeOneLineDel(bufferStr);
		}
		else if (data == "@parts")
		{
			std::string name ="NULL";
			while (1)
			{
				std::cout << "acual buffer1      :" << bufferStr << std::endl;
				name = resource.decodeOneLineDel(bufferStr);
				if (name == "")
					break;

				std::cout << "acual buffer2      :" << bufferStr << std::endl;
				sf::Vector2f pos;
				pos.x = stof(resource.decodeOneLineDel(bufferStr));
				pos.y = stof(resource.decodeOneLineDel(bufferStr));
				float degree = stof(resource.decodeOneLineDel(bufferStr));
				int unId = stoi(resource.decodeOneLineDel(bufferStr));
				player.parts.addPart(name, pos, degree, unId);
				std::cout << "acual buffer3      :" << bufferStr << std::endl;
			}
			
		}
		std::cout << "acual buffer at end     :" << bufferStr << std::endl;
	}
	std::cout << "buffer after load:" << bufferStr << std::endl;
}

bool LoadingDataStage::waitingForPlayers(std::string &str)
{


	return false;
}

bool LoadingDataStage::update(std::string &bufferStr)
{
	//How data looks like
	//playersID;@parts?eg; name; posx; posy; rot; Id; ...name;...@body_info;nameOfBody;

	auto & resource = ResourcesManager::getInstanceRef();

	int id = stoi(resource.decodeOneLineDel(bufferStr));
	//maybe do better search engine
	std::cout << "searching for player..." << std::endl;
	for (auto &player : resource.uniquePlayers)
	{
		if (player.uniqueId == id)
		{
			std::cout << "player found!"  << std::endl;
			loadPlayer(bufferStr, player);
			std::cout << "player loaded!" << std::endl;
			
			bufferStr = std::to_string(assignToSession(player)+2) + ";" + std::to_string(player.uniqueId) + ";";

			break;
		}
	}
	return true;
}

LoadingDataStage::~LoadingDataStage()
{
}
