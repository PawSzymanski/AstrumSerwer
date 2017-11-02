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
	//ACSESS CODE TO SERWER IS:5555

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
	
		std::string bufferStr = buffer;
		
		std::string tester = resource.decodeOneLineDel(bufferStr);
		//std::cout << "received test:" << (int)tester[0] << std::endl;
		if (tester != "5555" || bufferStr.size() < 2 || sender.toString() == "0.0.0.0" ) //safety features
			continue;
		std::cout << "received mess:" << bufferStr << std::endl;
		system("pause");
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
			//bufferStr = "5555;" + bufferStr;
			std::cout << "-1sending last message...:" << bufferStr << std::endl << "port:" << portToSendOrID << "sender:" << sender.toString() << ";" << std::endl;
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
			std::cout << "is waiting fo player?" << bufferStr <<std::endl;
			if (!serwer->waitingForPlayers(bufferStr))
			{
				std::cout << std::endl;
				std::cout << "no wait" << std::endl;
				std::cout << std::endl;
				for (auto player : resource.uniquePlayers)
					if (player.uniqueId == portToSendOrID)
					{
						serwer->update(bufferStr);
						bufferStr = "5555;" + std::to_string(actionCode) + ";" + std::to_string(portToSendOrID) + ";" + bufferStr;
						std::cout << "1sending last message...  :" << bufferStr  << " port:" << player.port << "sender:" << sender.toString() << ";" << std::endl;
						unsigned short poooort = player.port;
						socketSend.send(bufferStr.c_str(), bufferStr.size() + 1, sender, player.port);
						continue;
					}
				
			}
			else {
				for (auto player : resource.uniquePlayers)
					if (player.uniqueId == portToSendOrID)
					{
						
						bufferStr = "5555;" + std::to_string(actionCode) + ";" + bufferStr;//no variable " portToSendOrID " because of that tha it is not deleted anywhere
						unsigned short poooort = player.port;
						std::cout << "2sending last message...  :" << bufferStr << " port:" << player.port << "sender:" << sender.toString() << ";" << std::endl;
						socketSend.send(bufferStr.c_str(), bufferStr.size() + 1, sender, player.port);
						/*socket.unbind();
						socket.bind(22222);
						char b[100];
						std::string ippp;
						std::cout << "mess" << b << std::endl;
						socket.receive(b, sizeof(b), received, sender, portRec);
						std::cout << "mess" << b << std::endl;
						socket.unbind();
						socket.bind(portRec);*/
						std::cout << "mess:" << bufferStr << std::endl;
						std::cout << "port after:" << player.port << std::endl;
					}
			}
		}

	}

	
}