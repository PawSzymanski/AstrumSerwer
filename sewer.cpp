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
	sf::UdpSocket socket, socketSend;
	char buffer[512] = { 0 };
	std::size_t received;
	sf::IpAddress sender;
	unsigned short portRec = 52542;
	unsigned int portToSendOrID = 0;

	std::cout << "local adress: " << sender.getLocalAddress() << std::endl;
	std::cout << "public adress: " << sender.getPublicAddress() << std::endl;
	std::cout << "socket: " << std::to_string(portRec) << std::endl;

	auto & resource = ResourcesManager::getInstanceRef();
	int actionCode = 0;
	socket.bind(portRec); //REMEMBER THAT WE BIND ONLY LISTENER 
	

	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		//serwer.reset();
		//HOW MESSAGE LOOKS LIKE
		//number of action; id of player; ...
		//Number of action 1 is login stage, 2 is loading stage
		//Numbers of action from 3 and further are session numbers
		//std::cout << std::endl;
		//std::cout << "Waiting for message..." << std::endl;

		socket.receive(buffer, sizeof(buffer), received, sender, portRec);
	
		std::cout << "received mess:" << buffer << std::endl;

		std::string bufferStr = buffer;
		
		std::string tester = resource.decodeOneLineRead(bufferStr);
		std::cout << "received test:" << tester << std::endl;
		if (bufferStr.size() < 2 || sender.toString() == "0.0.0.0" || tester[0] > 39 || tester[0] < 30)
			continue;

		actionCode = stoi( resource.decodeOneLineDel(bufferStr) );
		//We will need to search for port to send message
		portToSendOrID = stoi(resource.decodeOneLineRead(bufferStr));

		//std::cout << "received act:"  << actionCode << "ID:"<< portToSendOrID <<std::endl;

		if (actionCode == 1)
		{
			serwer = &loginSt;     //= std::make_shared<LoginStage>(loginSt);
			//std::cout << "acual buffer:" << bufferStr << std::endl;

			serwer->update(bufferStr);
			// Send an answer
			//std::cout << "sending last message...  :" << bufferStr << std::endl << "port:" << portToSendOrID << "sender:" << sender.toString() << ";" << std::endl;
			socketSend.send(bufferStr.c_str(), bufferStr.size(), sender, portToSendOrID);
		}
		else if (actionCode == 2)
		{
			serwer = &loadingSt;	// std::make_shared<LoadingDataStage>(loadingSt);
			serwer->update(bufferStr);
			for (auto player : resource.uniquePlayers)
			{
				//std::cout << player.uniqueId << " " << portToSendOrID << std::endl;
				if (player.uniqueId == portToSendOrID)
				{
					//std::cout << "sending last message...  :" << bufferStr << std::endl << "port:" << player.port << "sender:" << sender.toString() << ";" << std::endl;
					socketSend.send(bufferStr.c_str(), bufferStr.size() + 1, sender, player.port);
					continue;
				}
				
			}
		}
		else if (actionCode > 2 && actionCode < 30)
		{
			serwer = &gamePlaySt[actionCode - 3];//std::make_shared<GamePlayStage>(gamePlaySt);//actionCode - 3]);
			std::cout << "is waiting fo player?" << bufferStr <<std::endl;
			if (!serwer->waitingForPlayers(bufferStr))
			{
				std::cout << "no wait" << std::endl;
				for (auto player : resource.uniquePlayers)
					if (player.uniqueId == portToSendOrID)
					{
						bufferStr = std::to_string(actionCode) + ";" + bufferStr;
						//std::cout << "sending last message...  :" << bufferStr  << " port:" << player.port << "sender:" << sender.toString() << ";" << std::endl;
						unsigned short poooort = player.port;
						socketSend.send(bufferStr.c_str(), bufferStr.size() + 1, sender, poooort);
						continue;
					}
				
			}
			else {
				for (auto player : resource.uniquePlayers)
					if (player.uniqueId == portToSendOrID)
					{
						bufferStr = std::to_string(actionCode) + ";" + std::to_string(portToSendOrID) + ";";
						unsigned short poooort = player.port;
						//std::cout << "sending last message...  :" << bufferStr << " port:" << player.port << "sender:" << sender.toString() << ";" << std::endl;
						socketSend.send(bufferStr.c_str(), bufferStr.size() + 1, sender, poooort);
					}
			}
		}

	}

	
}