#include "LibsAndDeclarations.h"
#include "SerwerStage.h"

#include "LoginStage.h"
#include "LoadingDataStage.h"
#include "GameplayStage.h"
#include "ResourceManager.h"


int main()
{
	LoginStage loginSt;
	std::vector<GamePlayStage> gamePlaySt;
	LoadingDataStage loadingSt(gamePlaySt);
	//GamePlayStage gamePlaySt;
	//std::shared_ptr<SerwerStage> serwer;
	SerwerStage * serwer ;
	serwer = &loginSt;
	sf::UdpSocket socket;
	char buffer[512] = { 0 };
	std::size_t received;
	sf::IpAddress sender;
	unsigned short port = 556;

	std::cout << "local adress: " << sender.getLocalAddress() << std::endl;
	std::cout << "public adress: " << sender.getPublicAddress() << std::endl;
	std::cout << "socket: " << std::to_string(port) << std::endl;

	auto & resource = ResourcesManager::getInstanceRef();
	int actionCode = 0;
	socket.bind(port);

	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		//serwer.reset();
		//HOW MESSAGE LOOKS LIKE
		//number of action; id of player; ...
		//Number of action 1 is login stage, 2 is loading stage
		//Numbers of action from 3 and further are session numbers

		std::cout << std::endl;
		std::cout << "Waiting for message..." << std::endl;

		socket.receive(buffer, sizeof(buffer), received, sender, port);
		std::cout << "received mess:" << actionCode << buffer << std::endl;
		std::string bufferStr = buffer;
		
		actionCode = stoi( resource.decodeOneLineDel(bufferStr) );
		std::cout << "received act:"  << actionCode << std::endl;

		if (actionCode == 1)
		{
			serwer = &loginSt;//= std::make_shared<LoginStage>(loginSt);
		}
		else if (actionCode == 2)
		{
			serwer = &loadingSt;// std::make_shared<LoadingDataStage>(loadingSt);
		}
		else if (actionCode > 2)
		{
			serwer = &gamePlaySt[actionCode - 3];//std::make_shared<GamePlayStage>(gamePlaySt);//actionCode - 3]);
			if (serwer->waitingForPlayers(bufferStr))
			{
				socket.send(bufferStr.c_str(), bufferStr.size() + 1, sender, port);
				continue;
			}
		}
		std::cout << "acual buffer:" << bufferStr << std::endl;
		
		serwer->update(bufferStr);
		// Send an answer
		std::cout << "sending last message...  :" << bufferStr << std::endl;
		socket.send(bufferStr.c_str(), bufferStr.size() + 1, sender, port);
	}

	
}