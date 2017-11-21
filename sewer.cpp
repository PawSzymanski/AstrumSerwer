#include "LibsAndDeclarations.h"
#include "SerwerStage.h"

#include "LoginStage.h"
#include "LoadingDataStage.h"
#include "GameplayStage.h"
#include "ResourceManager.h"

//ACSESS CODE TO SERWER IS:5555
int main()
{
	unsigned short portRec = 52542;
	unsigned int portToSendOrID = 0;

	int actionCode = 0;

	auto & resource = ResourcesManager::getInstanceRef();
	
	sf::IpAddress sender;
	std::size_t received;
	sf::UdpSocket socket, socketSend;

	//GamePlayStage gamePlaySt;
	//std::shared_ptr<SerwerStage> serwer;

	char buffer[512] = { 0 };

	SerwerStage * serwer;

	std::vector<GamePlayStage> gamePlaySt;
	LoginStage loginSt;
	LoadingDataStage loadingSt(gamePlaySt);

	serwer = &loginSt;
																		
	sf::Time timeElapsed = sf::Time::Zero;
	sf::Clock clk;
	//REMEMBER THAT WE BIND ONLY LISTENER 
	socket.bind(portRec);

	std::cout << "local adress: " << sender.getLocalAddress() << std::endl;
	std::cout << "public adress: " << sender.getPublicAddress() << std::endl;
	std::cout << "socket: " << std::to_string(portRec) << std::endl;
	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		//	HOW MESSAGE LOOKS LIKE
		//	number of action; id of player; ...
		//	Number of action 1 is login stage, 2 is loading stage
		//	Numbers of action from 3 and further are session numbers
		
		//	std::cout << std::endl;
		//	std::cout << "Waiting for message..." << std::endl;
		/////////////////////////////////////////////////////////////////////
		socket.receive(buffer, sizeof(buffer), received, sender, portRec);///
		/////////////////////////////////////////////////////////////////////

		clk.restart();
		timeElapsed = sf::Time::Zero;
		std::string bufferStr = buffer;
		//read the first part of message looking for passworld to server
		std::string tester = resource.decodeOneLineDel(bufferStr);
		//check password
		if (bufferStr.size() < 9 || tester != "5555" || sender.toString() == "0.0.0.0" ) //safety features
			continue;
		
		//std::cout << "received mess:" << bufferStr << std::endl;
		actionCode = stoi( resource.decodeOneLineDel(bufferStr) );
		//system("pause");
		//We will need to search for port to send message
		portToSendOrID = stoi(resource.decodeOneLineRead(bufferStr));

		if (actionCode == 1)
		{
			serwer = &loginSt;     //= std::make_shared<LoginStage>(loginSt);

			serwer->update(bufferStr);
			// Send an answer
			std::cout << "-1sending last message...:" << bufferStr << std::endl << "port:" << portToSendOrID << "sender:" << sender.toString() << ";" << std::endl;
			socketSend.send(bufferStr.c_str(), bufferStr.size(), sender, portToSendOrID);
		}
		else if (actionCode == 2)
		{
			serwer = &loadingSt;	// std::make_shared<LoadingDataStage>(loadingSt);
			serwer->update(bufferStr);
			for (auto player : resource.uniquePlayers)
			{
				if (player.uniqueId == portToSendOrID)
				{
					bufferStr = "5555;" + bufferStr;
					std::cout << "0sending last message...  :" << bufferStr << std::endl << "port:" << player.port << "sender:" << sender.toString() << ";" << std::endl;
					socketSend.send(bufferStr.c_str(), bufferStr.size() + 1, sender, player.port);
					continue;
				}
			}
		}
		else if (actionCode > 2 && actionCode < 30)
		{
			serwer = &gamePlaySt[actionCode - 3];//std::make_shared<GamePlayStage>(gamePlaySt);//actionCode - 3]);
			if (!serwer->waitingForPlayers(bufferStr))
			{
				for (auto player : resource.uniquePlayers)
					if (player.uniqueId == portToSendOrID)
					{
						serwer->update(bufferStr);
						bufferStr = "5555;" + std::to_string(actionCode) + ";" + std::to_string(portToSendOrID) + ";" + bufferStr;
						//std::cout <<"in game: "<< bufferStr << std::endl;
						unsigned short poooort = player.port;
						socketSend.send(bufferStr.c_str(), bufferStr.size() + 1, sender, player.port);
						continue;
					}
			}
			else {
				for (auto player : resource.uniquePlayers)
					if (player.uniqueId == portToSendOrID)
					{
						bufferStr = "5555;" + std::to_string(actionCode) + ";" + bufferStr;//no variable " portToSendOrID " because it is not deleted anywhere
						unsigned short poooort = player.port;
						//std::cout << "2sending last message...  :" << bufferStr << " port:" << player.port << "sender:" << sender.toString() << ";" << std::endl;
						socketSend.send(bufferStr.c_str(), bufferStr.size() + 1, sender, player.port);
					}
			}
			//system("pause");
		}
		timeElapsed = clk.restart();
		std::cout << "time elapsed:" << timeElapsed.asMilliseconds() << std::endl;
	}
}